#include "scenemandelbrot.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::endl;
using std::cerr;

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneMandelbrot::SceneMandelbrot():
  width(800), height(600), dataBuf(0), fsQuad(0),
  center(0.001643721971153f, 0.822467633298876f), cheight(2.0f),
  time(0.0f), deltaT(0.0f), speed(200.0f), angle(0.0f), rotSpeed(60.0f)
{
}

void SceneMandelbrot::initScene()
{
  compileAndLinkShader();
  initBuffers();
  setWindow();

  glEnable(GL_DEPTH_TEST);

  renderProg.use();
  renderProg.setUniform("LightPosition", glm::vec4(0.0f,0.0f,0.0f,1.0f));
  renderProg.setUniform("LightIntensity", glm::vec3(1.0f) );
  renderProg.setUniform("Kd", glm::vec3(0.8f));
  renderProg.setUniform("Ka", glm::vec3(0.2f));
  renderProg.setUniform("Ks", glm::vec3(0.2f));
  renderProg.setUniform("Shininess", 80.0f);

  cube = new VBOCube();
}

void SceneMandelbrot::setWindow() {
  computeProg.use();
  float ar = 1.0;
  float cwidth = cheight * ar;

  glm::vec4 bbox( center.x - cwidth/2.0f, center.y - cheight/2.0f,
      center.x + cwidth/2.0f, center.y + cheight/2.0f);
  computeProg.setUniform("CompWindow",bbox );
}

void SceneMandelbrot::initBuffers()
{
  // The buffer for the colors, as an image texture
  GLuint imgTex;
  glGenTextures(1, &imgTex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, imgTex);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 256, 256);
  glBindImageTexture(0, imgTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
}

void SceneMandelbrot::update( float t )
{
  if( time == 0.0f ) {
    deltaT = 0.0f;
  } else {
    deltaT = t - time;
  }
  time = t;

  if( !animating() ) return;

  float dy = cheight / height;

  cheight -= deltaT * speed * dy;
  setWindow();
  angle += rotSpeed * deltaT;
  if( angle > 360.0f ) angle -= 360.0f;
}

void SceneMandelbrot::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  computeProg.use();
  glDispatchCompute(256/32, 256/32, 1);
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  renderProg.use();
  glm::mat4 view = glm::lookAt( glm::vec3(0,0,2), glm::vec3(0,0,0), glm::vec3(0,1,0) );
  glm::mat4 model = glm::rotate( glm::mat4(1.0f), glm::radians(angle), glm::vec3(1,1.5f,0.5f));
  glm::mat4 mv = view * model;
  glm::mat3 norm =  mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );
  glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)width/height, 1.0f, 100.0f);

  renderProg.setUniform("ModelViewMatrix", mv);
  renderProg.setUniform("NormalMatrix", norm);
  renderProg.setUniform("MVP", proj * mv);

  cube->render();
}

void SceneMandelbrot::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneMandelbrot::compileAndLinkShader()
{
  try {
    renderProg.compileShader("shader/ads.vs");
    renderProg.compileShader("shader/ads.fs");
    renderProg.link();

    computeProg.compileShader("shader/mandelbrot.cs");
    computeProg.link();
  } catch(GLSLProgramException &e ) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
