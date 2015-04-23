#include "sceneparticles.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::endl;
using std::cerr;

#include "glutils.h"

#include <vector>
using std::vector;

using glm::vec3;

#include "tgaio.h"

#include <glm/gtc/matrix_transform.hpp>

#define PRIM_RESTART 0xffffff

SceneParticles::SceneParticles():
  width(800), height(600),
  nParticles(100,100,100),
  time(0.0f), deltaT(0.0f), speed(35.0f), angle(0.0f),
  bh1(5,0,0,1), bh2(-5,0,0,1)
{
  totalParticles = nParticles.x * nParticles.y * nParticles.z;
}

void SceneParticles::initScene()
{
  compileAndLinkShader();
  initBuffers();

  glClearColor(1,1,1,1);

  projection = glm::perspective( glm::radians(50.0f), (float)width/height, 1.0f, 100.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void SceneParticles::initBuffers()
{
  // Initial positions of the particles
  vector<GLfloat> initPos;
  vector<GLfloat> initVel(totalParticles * 4, 0.0f);
  glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
  GLfloat dx = 2.0f / (nParticles.x - 1),
          dy = 2.0f / (nParticles.y - 1),
          dz = 2.0f / (nParticles.z - 1);
  // We want to center the particles at (0,0,0)
  glm::mat4 transf = glm::translate(glm::mat4(1.0f), glm::vec3(-1,-1,-1));
  for( int i = 0; i < nParticles.x; i++ ) {
    for( int j = 0; j < nParticles.y; j++ ) {
      for( int k = 0; k < nParticles.z; k++ ) {
        p.x = dx * i;
        p.y = dy * j;
        p.z = dz * k;
        p.w = 1.0f;
        p = transf * p;
        initPos.push_back(p.x);
        initPos.push_back(p.y);
        initPos.push_back(p.z);
        initPos.push_back(p.w);
      }
    }
  }

  // We need buffers for position , and velocity.
  GLuint bufs[2];
  glGenBuffers(2, bufs);
  GLuint posBuf = bufs[0];
  GLuint velBuf = bufs[1];

  GLuint bufSize = totalParticles * 4 * sizeof(GLfloat);

  // The buffers for positions
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
  glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, &initPos[0], GL_DYNAMIC_DRAW);

  // Velocities
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
  glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, &initVel[0], GL_DYNAMIC_COPY);

  // Set up the VAO
  glGenVertexArrays(1, &particlesVao);
  glBindVertexArray(particlesVao);

  glBindBuffer(GL_ARRAY_BUFFER, posBuf);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // Set up a buffer and a VAO for drawing the attractors (the "black holes")
  glGenBuffers(1, &bhBuf);
  glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
  GLfloat data[] = { bh1.x, bh1.y, bh1.z, bh1.w, bh2.x, bh2.y, bh2.z, bh2.w };
  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

  glGenVertexArrays(1, &bhVao);
  glBindVertexArray(bhVao);

  glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

void SceneParticles::update( float t )
{
  if( time == 0.0f ) {
    deltaT = 0.0f;
  } else {
    deltaT = t - time;
  }
  time = t;
  if( animating() ) {
    angle += speed * deltaT;
    if( angle > 360.0f ) angle -= 360.0f;
  }
}

void SceneParticles::render()
{
  // Rotate the attractors ("black holes")
  glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0,0,1));
  glm::vec3 att1 = glm::vec3(rot*bh1);
  glm::vec3 att2 = glm::vec3(rot*bh2);

  // Execute the compute shader
  computeProg.use();
  computeProg.setUniform("BlackHolePos1", att1);
  computeProg.setUniform("BlackHolePos2", att2);
  glDispatchCompute(totalParticles / 1000, 1, 1);
  glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

  // Draw the scene
  renderProg.use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  view = glm::lookAt(glm::vec3(2,0,20), glm::vec3(0,0,0), glm::vec3(0,1,0));
  model = glm::mat4(1.0f);
  setMatrices();

  // Draw the particles
  glPointSize(1.0f);
  renderProg.setUniform("Color", glm::vec4(0,0,0,0.2f));
  glBindVertexArray(particlesVao);
  glDrawArrays(GL_POINTS,0, totalParticles);
  glBindVertexArray(0);

  // Draw the attractors
  glPointSize(5.0f);
  GLfloat data[] = { att1.x, att1.y, att1.z, 1.0f, att2.x, att2.y, att2.z, 1.0f };
  glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
  glBufferSubData( GL_ARRAY_BUFFER, 0, 8 * sizeof(GLfloat), data );
  renderProg.setUniform("Color", glm::vec4(1,1,0,1.0f));
  glBindVertexArray(bhVao);
  glDrawArrays(GL_POINTS, 0, 2);
  glBindVertexArray(0);

}

void SceneParticles::setMatrices() {
  renderProg.use();
  glm::mat4 mv = view * model;
  glm::mat3 norm =  mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );

  renderProg.setUniform("ModelViewMatrix", mv);
  renderProg.setUniform("NormalMatrix", norm);
  renderProg.setUniform("MVP", projection * mv);
}

void SceneParticles::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneParticles::compileAndLinkShader()
{
  try {
    renderProg.compileShader("shader/particles.vs");
    renderProg.compileShader("shader/particles.fs");
    renderProg.link();

    computeProg.compileShader("shader/particles.cs");
    computeProg.link();
  } catch(GLSLProgramException &e ) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
