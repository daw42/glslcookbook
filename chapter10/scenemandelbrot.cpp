#include "scenemandelbrot.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::endl;
using std::cerr;

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneMandelbrot::SceneMandelbrot(): 
  width(800), height(600), dataBuf(0), fsQuad(0),
  center(0.001643721971153f, 0.822467633298876f), cheight(2.0f),
  time(0.0f), deltaT(0.0f), speed(200.0f)
{
}

void SceneMandelbrot::initScene()
{    
  compileAndLinkShader();
  initBuffers();
  setWindow();
}

void SceneMandelbrot::setWindow() {
  computeProg.use();
  float ar = (float)width/ height;
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
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height); 
  glBindImageTexture(0, imgTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
  
  // Create vertex array for screen filling quad 
  GLfloat pos[] = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
  GLuint quadBuf;
  glGenBuffers(1, &quadBuf);
  glBindBuffer(GL_ARRAY_BUFFER, quadBuf);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), pos, GL_STATIC_DRAW);

  glGenVertexArrays(1, &fsQuad);
  glBindVertexArray(fsQuad);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

void SceneMandelbrot::update( float t )
{
  if( time == 0.0f ) {
    deltaT = 0.0f;
  } else {
    deltaT = t - time;
  }
  time = t;

  float dy = cheight / height;

  cheight -= deltaT * speed * dy;
  setWindow();
}

void SceneMandelbrot::render()
{
  GLUtils::checkForOpenGLError(__FILE__,__LINE__);
  computeProg.use();
  glDispatchCompute(width/10, height/10, 1);
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  renderProg.use();
  glBindVertexArray(fsQuad);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4 );
  glBindVertexArray(0);
  GLUtils::checkForOpenGLError(__FILE__,__LINE__);

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
    GLUtils::checkForOpenGLError(__FILE__,__LINE__);

    renderProg.compileShader("shader/mandelbrot.vs");
    renderProg.compileShader("shader/mandelbrot.fs");
    renderProg.link();

    computeProg.compileShader("shader/mandelbrot.cs");
    computeProg.link();
    GLUtils::checkForOpenGLError(__FILE__,__LINE__);


  } catch(GLSLProgramException &e ) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
