#include "sceneoit.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"

#include <iostream>
using std::endl;
using std::cerr;

using glm::vec3;

#include <vector>
using std::vector;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

SceneOit::SceneOit() : width(800), height(600), angle(0.0f), tPrev(0.0f),
rotSpeed(glm::pi<float>() / 8.0f)
{ }

void SceneOit::initScene()
{
  compileAndLinkShader();
  animate(false);
  glClearColor(0.5f,0.5f,0.5f,1.0f);

  glEnable(GL_DEPTH_TEST);

  cube = new VBOCube();
  sphere = new VBOSphere(1.0f, 40, 40);

  angle = glm::radians(210.0f);

  //  glEnable(GL_BLEND);
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  initShaderStorage();

  GLuint programHandle = prog.getHandle();
  pass1Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass1");
  pass2Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass2");

  // Set up a  VAO for the full-screen quad
  GLfloat verts[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
  GLuint bufHandle;
  glGenBuffers(1, &bufHandle);
  glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
  glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

  // Set up the vertex array object
  glGenVertexArrays( 1, &fsQuad );
  glBindVertexArray(fsQuad);

  glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray(0);  // Vertex position

  glBindVertexArray(0);
}


void SceneOit::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;
  if( animating() ) {
    angle += deltaT * rotSpeed;
    if( angle > 360.0 ) angle -= 360.0f;
  }
}

void SceneOit::render()
{
  clearBuffers();
  pass1();
  glFlush();
  pass2();
}

void SceneOit::pass1() {
  glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass1Index);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  view = glm::lookAt(vec3(11.0f * cos(angle),2.0f,11.0f * sin(angle)), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));

  projection = glm::perspective( glm::radians(50.0f), (float)width/height, 1.0f, 1000.0f);

  glDepthMask( GL_FALSE );

  // draw scene
  drawScene();

  glFinish();
}

void SceneOit::pass2() {
  glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

  glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass2Index);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);
  model = glm::mat4(1.0f);

  setMatrices();

  // Draw a screen filler
  glBindVertexArray(fsQuad);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);
}

void SceneOit::clearBuffers() {
  GLuint zero = 0;
  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, buffers[COUNTER_BUFFER] );
  glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
  glBindTexture(GL_TEXTURE_2D, headPtrTex);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED_INTEGER,
      GL_UNSIGNED_INT, NULL);
}

void SceneOit::setMatrices()
{
  mat4 mv = view * model;

  prog.setUniform("NormalMatrix",
      mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ) );
  prog.setUniform("ModelViewMatrix", mv);
  prog.setUniform("MVP", projection * mv);
}

void SceneOit::resize(int w, int h)
{
  glViewport(0,0,w,h);
  width = w;
  height = h;
}

void SceneOit::compileAndLinkShader()
{
  try {
    prog.compileShader("shader/oit.vs");
    prog.compileShader("shader/oit.fs");
    prog.link();
    prog.use();
  } catch(GLSLProgramException &e ) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}

void SceneOit::drawScene() {
  prog.setUniform("LightPosition", vec4(0,0,0,1) );
  prog.setUniform("LightIntensity", vec3(0.9f) );

  prog.setUniform("Kd", vec4(0.2f, 0.2f, 0.9f, 0.55f) );
  float size = 0.45f;
  for( int i = 0; i <= 6; i++ )
    for( int j = 0; j <= 6; j++ )
      for( int k = 0; k <= 6; k++ ) {
        if( (i + j + k) % 2 == 0 ) {
          model = glm::translate(mat4(1.0f), vec3(i-3, j-3, k-3));
          model = glm::scale( model, vec3(size) );
          setMatrices();
          sphere->render();
        }
      }

  prog.setUniform("Kd", vec4(0.9f, 0.2f, 0.2f, 0.4f) );
  size = 2.0f;
  float pos = 1.75f;
  model = glm::translate(mat4(1.0f), vec3(-pos, -pos, pos) );
  model = glm::scale( model, vec3(size) );
  setMatrices();
  cube->render();
  model = glm::translate(mat4(1.0f), vec3(-pos, -pos, -pos) );
  model = glm::scale( model, vec3(size) );
  setMatrices();
  cube->render();
  model = glm::translate(mat4(1.0f), vec3(-pos, pos, pos) );
  model = glm::scale( model, vec3(size) );
  setMatrices();
  cube->render();
  model = glm::translate(mat4(1.0f), vec3(-pos, pos, -pos) );
  model = glm::scale( model, vec3(size) );
  setMatrices();
  cube->render();
  model = glm::translate(mat4(1.0f), vec3(pos, pos, pos) );
  model = glm::scale( model, vec3(size) );
  setMatrices();
  cube->render();
  model = glm::translate(mat4(1.0f), vec3(pos, pos, -pos) );
  model = glm::scale( model, vec3(size) );
  setMatrices();
  cube->render();
  model = glm::translate(mat4(1.0f), vec3(pos, -pos, pos) );
  model = glm::scale( model, vec3(size) );
  setMatrices();
  cube->render();
  model = glm::translate(mat4(1.0f), vec3(pos, -pos, -pos) );
  model = glm::scale( model, vec3(size) );
  setMatrices();
  cube->render();
}

void SceneOit::initShaderStorage()
{
  glGenBuffers(2, buffers);
  GLuint maxNodes = 20 * width * height;
  GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint); // The size of a linked list node

  // Our atomic counter
  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, buffers[COUNTER_BUFFER]);
  glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

  // The buffer for the head pointers, as an image texture
  glGenTextures(1, &headPtrTex);
  glBindTexture(GL_TEXTURE_2D, headPtrTex);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, width, height);
  glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

  // The buffer of linked lists
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[LINKED_LIST_BUFFER]);
  glBufferData(GL_SHADER_STORAGE_BUFFER, maxNodes * nodeSize, NULL, GL_DYNAMIC_DRAW);

  prog.setUniform("MaxNodes", maxNodes);

  vector<GLuint> headPtrClearBuf(width*height, 0xffffffff);
  glGenBuffers(1, &clearBuf);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClearBuf.size() * sizeof(GLuint),
      &headPtrClearBuf[0], GL_STATIC_COPY);
}
