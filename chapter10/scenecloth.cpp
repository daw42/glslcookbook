#include "scenecloth.h"

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

SceneCloth::SceneCloth():
  width(800), height(600), clothVao(0), numElements(0),
  nParticles(40,40), clothSize(4.0f, 3.0f),
  time(0.0f), deltaT(0.0f), speed(200.0f),
  readBuf(0)
{
}

void SceneCloth::initScene()
{
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(PRIM_RESTART);

  compileAndLinkShader();
  initBuffers();

  projection = glm::perspective( glm::radians(50.0f), (float)width/height, 1.0f, 100.0f);

  renderProg.use();
  renderProg.setUniform("LightPosition", glm::vec4(0.0f,0.0f,0.0f,1.0f));
  renderProg.setUniform("LightIntensity", glm::vec3(1.0f) );
  renderProg.setUniform("Kd", glm::vec3(0.8f));
  renderProg.setUniform("Ka", glm::vec3(0.2f));
  renderProg.setUniform("Ks", glm::vec3(0.2f));
  renderProg.setUniform("Shininess", 80.0f);

  computeProg.use();
  float dx = clothSize.x / (nParticles.x-1) ;
  float dy = clothSize.y / (nParticles.y-1) ;
  computeProg.setUniform("RestLengthHoriz", dx );
  computeProg.setUniform("RestLengthVert", dy );
  computeProg.setUniform("RestLengthDiag", sqrtf(dx * dx + dy * dy));

  glActiveTexture(GL_TEXTURE0);
  TGAIO::loadTex("../media/texture/me_textile.tga");
}

void SceneCloth::initBuffers()
{
  // Initial transform
  glm::mat4 transf = glm::translate(glm::mat4(1.0), glm::vec3(0,clothSize.y,0));
  transf = glm::rotate(transf, glm::radians(-80.0f), glm::vec3(1,0,0));
  transf = glm::translate(transf, glm::vec3(0,-clothSize.y,0));

  // Initial positions of the particles
  vector<GLfloat> initPos;
  vector<GLfloat> initVel(nParticles.x * nParticles.y * 4, 0.0f);
  vector<float> initTc;
  float dx = clothSize.x / (nParticles.x-1) ;
  float dy = clothSize.y / (nParticles.y-1) ;
  float ds = 1.0f / (nParticles.x - 1);
  float dt = 1.0f / (nParticles.y - 1);
  glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
  for( int i = 0; i < nParticles.y; i++ ) {
    for( int j = 0; j < nParticles.x; j++ ) {
      p.x = dx * j;
      p.y = dy * i;
      p.z = 0.0f;
      p = transf * p;
      initPos.push_back(p.x);
      initPos.push_back(p.y);
      initPos.push_back(p.z);
      initPos.push_back(1.0f);

      initTc.push_back(ds * j);
      initTc.push_back(dt * i);
    }
  }

  // Every row is one triangle strip
  vector<GLuint> el;
  for( int row = 0; row < nParticles.y - 1; row++ ) {
    for( int col = 0; col < nParticles.x; col++ ) {
      el.push_back( (row+1) * nParticles.x + (col  ) );
      el.push_back( (row  ) * nParticles.x + (col  ) );
    }
    el.push_back(PRIM_RESTART);
  }

  // We need buffers for position (2), element index,
  // velocity (2), normal, and texture coordinates.
  GLuint bufs[7];
  glGenBuffers(7, bufs);
  posBufs[0] = bufs[0];
  posBufs[1] = bufs[1];
  velBufs[0] = bufs[2];
  velBufs[1] = bufs[3];
  normBuf = bufs[4];
  elBuf = bufs[5];
  tcBuf = bufs[6];

  GLuint parts = nParticles.x * nParticles.y;

  // The buffers for positions
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBufs[0]);
  glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), &initPos[0], GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, posBufs[1]);
  glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

  // Velocities
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velBufs[0]);
  glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), &initVel[0], GL_DYNAMIC_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, velBufs[1]);
  glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);

  // Normal buffer
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, normBuf);
  glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);

  // Element indicies
  glBindBuffer(GL_ARRAY_BUFFER, elBuf);
  glBufferData(GL_ARRAY_BUFFER, el.size() * sizeof(GLuint), &el[0], GL_DYNAMIC_COPY);

  // Texture coordinates
  glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
  glBufferData(GL_ARRAY_BUFFER, initTc.size() * sizeof(GLfloat), &initTc[0], GL_STATIC_DRAW);

  numElements = GLuint(el.size());

  // Set up the VAO
  glGenVertexArrays(1, &clothVao);
  glBindVertexArray(clothVao);

  glBindBuffer(GL_ARRAY_BUFFER, posBufs[0]);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, normBuf);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elBuf);
  glBindVertexArray(0);
}

void SceneCloth::update( float t )
{
  if( time == 0.0f ) {
    deltaT = 0.0f;
  } else {
    deltaT = t - time;
  }
  time = t;

}

void SceneCloth::render()
{
  computeProg.use();
  for( int i = 0; i < 1000; i++ ) {
    glDispatchCompute(nParticles.x / 10, nParticles.y/10, 1);
    glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

    // Swap buffers
    readBuf = 1 - readBuf;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBufs[readBuf]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, posBufs[1-readBuf]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velBufs[readBuf]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, velBufs[1-readBuf]);
  }

  // Compute the normals
  computeProgNorm.use();
  glDispatchCompute(nParticles.x/10, nParticles.y/10, 1);
  glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

  // Now draw the scene
  renderProg.use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  view = glm::lookAt(glm::vec3(3,2,5), glm::vec3(2,1,0), glm::vec3(0,1,0));
  model = glm::mat4(1.0f);
  setMatrices();

  // Draw the cloth
  glBindVertexArray(clothVao);
  glDrawElements(GL_TRIANGLE_STRIP, numElements, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

}

void SceneCloth::setMatrices() {
  renderProg.use();
  glm::mat4 mv = view * model;
  glm::mat3 norm =  mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );

  renderProg.setUniform("ModelViewMatrix", mv);
  renderProg.setUniform("NormalMatrix", norm);
  renderProg.setUniform("MVP", projection * mv);
}

void SceneCloth::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneCloth::compileAndLinkShader()
{
  try {
    renderProg.compileShader("shader/ads.vs");
    renderProg.compileShader("shader/ads.fs");
    renderProg.link();

    computeProg.compileShader("shader/cloth.cs");
    computeProg.link();

    computeProgNorm.compileShader("shader/cloth_normal.cs");
    computeProgNorm.link();
  } catch(GLSLProgramException &e ) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
