#include "sceneprojtex.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "tgaio.h"

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

SceneProjTex::SceneProjTex() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f) { }

void SceneProjTex::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(14, mat4(1.0f));
    plane = new VBOPlane(100.0f,100.0f,1,1);

    projection = mat4(1.0f);

    angle = glm::radians(90.0f);

    vec3 projPos = vec3(2.0f,5.0f,5.0f);
    vec3 projAt = vec3(-2.0f,-4.0f,0.0f);
    vec3 projUp = vec3(0.0f,1.0f,0.0f);
    mat4 projView = glm::lookAt(projPos, projAt, projUp);
    mat4 projProj = glm::perspective(glm::radians(30.0f), 1.0f, 0.2f, 1000.0f);
    mat4 projScaleTrans = glm::translate(mat4(), vec3(0.5f)) * glm::scale(mat4(), vec3(0.5f));
    prog.setUniform("ProjectorMatrix", projScaleTrans * projProj * projView);

    // Load texture file
    glActiveTexture(GL_TEXTURE0);
    TGAIO::loadTex("../media/texture/flower.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    prog.setUniform("Light.Position", vec4(0.0f,0.0f,0.0f,1.0f) );
    prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f));

#ifdef __APPLE__
    prog.setUniform("ProjectorTex", 0);
#endif
}

void SceneProjTex::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;

  angle += rotSpeed * deltaT;
  if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneProjTex::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3( 7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));

    prog.setUniform("Material.Kd", 0.5f, 0.2f, 0.1f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-1.0f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-0.75f,0.0f));
    setMatrices();
    plane->render();
}

void SceneProjTex::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelMatrix", model);
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneProjTex::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(50.0f), (float)w/h, 0.3f, 1000.0f);
}

void SceneProjTex::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
    prog.compileShader("shader/projtex_41.vs");
    prog.compileShader("shader/projtex_41.fs");
#else
    prog.compileShader("shader/projtex.vs");
    prog.compileShader("shader/projtex.fs");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
