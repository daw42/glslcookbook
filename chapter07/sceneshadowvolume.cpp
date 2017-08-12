#include "sceneshadowvolume.h"

#include <cstdio>

#include "glutils.h"
#include "tgaio.h"

using glm::vec3;

#include <iostream>
using std::cerr;
using std::endl;
#include <cstdlib>

#include <glm/gtc/matrix_transform.hpp>

SceneShadowVolume::SceneShadowVolume() : width(800), height(600), rotSpeed(0.1f), tPrev(0)
{ }

void SceneShadowVolume::initScene()
{
  compileAndLinkShader();

  glClearColor(0.5f,0.5f,0.5f,1.0f);
  glClearStencil(0);

  glEnable(GL_DEPTH_TEST);

  angle = 0.0f;

  plane = new VBOPlane(10.0f, 10.0f, 2, 2, 5.0f, 5.0f);
  spot = new VBOMeshAdj("../media/spot/spot_triangulated.obj");

  // Set up the framebuffer object
  setupFBO();

  renderProg.use();
  renderProg.setUniform("LightIntensity", vec3(1.0f));

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

  //Load textures
  glActiveTexture(GL_TEXTURE2);
  spotTex = TGAIO::loadTex("../media/spot/spot_texture.tga");
  brickTex = TGAIO::loadTex("../media/texture/brick1.tga");

  updateLight();

    renderProg.use();
    renderProg.setUniform("Tex", 2);

    compProg.use();
    compProg.setUniform("DiffSpecTex", 0);

  this->animate(true);
}

void SceneShadowVolume::updateLight() {
  lightPos = vec4(5.0f * vec3(cosf(angle) * 7.5f, 1.5f, sinf(angle) * 7.5f), 1.0f);  // World coords
}

// The FBO has one texture for the diffuse + specular
// components.
void SceneShadowVolume::setupFBO()
{
    // The depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf );
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // The ambient buffer
    GLuint ambBuf;
    glGenRenderbuffers(1, &ambBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, ambBuf );
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);

    // The diffuse+specular component
    glActiveTexture(GL_TEXTURE0);
    GLuint diffSpecTex;
    glGenTextures(1, &diffSpecTex);
    glBindTexture(GL_TEXTURE_2D, diffSpecTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create and set up the FBO
    glGenFramebuffers(1, &colorDepthFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ambBuf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffSpecTex, 0);

    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    } else {
        printf("Framebuffer is not complete.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void SceneShadowVolume::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;
  if( animating() ) {
    angle += deltaT * rotSpeed;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
    updateLight();
  }
}

void SceneShadowVolume::render()
{
  pass1();
    glFlush();
  pass2();
    glFlush();
  pass3();
}

// Just renders the geometry normally with shading.  The ambient component
// is rendered to one buffer, and the diffuse and specular componenets are
// written to a texture.
void SceneShadowVolume::pass1() {
  glDepthMask(GL_TRUE);
  glDisable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);
  projection = glm::infinitePerspective(glm::radians(50.0f), (float)width/height, 0.5f );
  view = glm::lookAt(vec3(5.0f, 5.0f, 5.0f), vec3(0,2,0), vec3(0,1,0) );

  renderProg.use();
  renderProg.setUniform("LightPosition", view * lightPos);

  glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  drawScene(renderProg, false);
}

// This is the pass that generates the shadow volumes using the
// geometry shader
void SceneShadowVolume::pass2() {
  volumeProg.use();
  volumeProg.setUniform("LightPosition", view * lightPos);

  // Copy the depth and color buffers from the FBO into the default FBO
  // The color buffer should contain the ambient component.
  glBindFramebuffer(GL_READ_FRAMEBUFFER, colorDepthFBO);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  glBlitFramebuffer(0,0,width-1,height-1,0,0,width-1,height-1,GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT,GL_NEAREST);

  // Disable writing to the color buffer and depth buffer
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  // Re-bind to the default framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Set up the stencil test so that it always succeeds, increments
  // for front faces, and decrements for back faces.
  glClear(GL_STENCIL_BUFFER_BIT);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 0, 0xffff);
  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);

  // Draw only the shadow casters
  drawScene(volumeProg, true);

  // Enable writing to the color buffer
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

// In this pass, we read the diffuse and specular component from a texture
// and combine it with the ambient component if the stencil test succeeds.
void SceneShadowVolume::pass3() {
  // We don't need the depth test
  glDisable(GL_DEPTH_TEST);

  // We want to just sum the ambient component and the diffuse + specular
  // when the stencil test succeeds, so we'll use this simple blend function.
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);

  // We want to only render those pixels that have a stencil value
  // equal to zero.
  glStencilFunc(GL_EQUAL, 0, 0xffff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  compProg.use();

  // Just draw a screen filling quad
  model = mat4(1.0f);
  projection = model;
  view = model;
  setMatrices(compProg);

  glBindVertexArray(fsQuad);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);

  // Restore some state
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

void SceneShadowVolume::drawScene(GLSLProgram &prog, bool onlyShadowCasters)
{
  vec3 color;

  if( ! onlyShadowCasters ) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, spotTex);
    color = vec3(1.0f);
    prog.setUniform("Ka", color * 0.1f);
    prog.setUniform("Kd", color);
    prog.setUniform("Ks", vec3(0.9f));
    prog.setUniform("Shininess", 150.0f);
  }

  model = mat4(1.0f);
  model = glm::translate(model, vec3(-2.3f,1.0f,0.2f));
  model = glm::rotate(model, glm::radians(180.0f), vec3(0.0f,1.0f,0.0f));
  model = glm::scale(model, vec3(1.5f));
  setMatrices(prog);
  spot->render();

  model = mat4(1.0f);
  model = glm::translate(model, vec3(2.5f,1.0f,-1.2f));
  model = glm::rotate(model, glm::radians(180.0f), vec3(0.0f,1.0f,0.0f));
  model = glm::scale(model, vec3(1.5f));
  setMatrices(prog);
  spot->render();

  model = mat4(1.0f);
  model = glm::translate(model, vec3(0.5f,1.0f,2.7f));
  model = glm::rotate(model, glm::radians(180.0f), vec3(0.0f,1.0f,0.0f));
  model = glm::scale(model, vec3(1.5f));
  setMatrices(prog);
  spot->render();

  if( !onlyShadowCasters ) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brickTex);
    color = vec3(0.5f);
    prog.setUniform("Kd", color);
    prog.setUniform("Ks", vec3(0.0f));
    prog.setUniform("Ka", vec3(0.1f));
    prog.setUniform("Shininess", 1.0f);
    model = mat4(1.0f);
    setMatrices(prog);
    plane->render();
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-5.0f,5.0f,0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(1,0,0));
    model = glm::rotate(model, glm::radians(-90.0f),vec3(0.0f,0.0f,1.0f));
    setMatrices(prog);
    plane->render();
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,5.0f,-5.0f));
    model = glm::rotate(model, glm::radians(90.0f),vec3(1.0f,0.0f,0.0f));
    setMatrices(prog);
    plane->render();
    model = mat4(1.0f);
  }
}

void SceneShadowVolume::setMatrices(GLSLProgram &prog)
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("ProjMatrix", projection);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
}

void SceneShadowVolume::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneShadowVolume::compileAndLinkShader()
{
    try {
      // The shader for the volumes
      volumeProg.compileShader("shader/shadowvolume-vol.vs");
      volumeProg.compileShader("shader/shadowvolume-vol.gs");
      volumeProg.compileShader("shader/shadowvolume-vol.fs");
      volumeProg.link();

      // The shader for rendering and compositing
      renderProg.compileShader("shader/shadowvolume-render.vs");
      renderProg.compileShader("shader/shadowvolume-render.fs");
      renderProg.link();

      // The final composite shader
      compProg.compileShader("shader/shadowvolume-comp.vs");
      compProg.compileShader("shader/shadowvolume-comp.fs");
      compProg.link();

    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
