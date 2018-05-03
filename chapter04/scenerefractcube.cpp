#include "scenerefractcube.h"
#include "texture.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::mat4;

SceneRefractCube::SceneRefractCube() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f),
                                       teapot(14, glm::mat4(1.0f)){ }

void SceneRefractCube::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

    angle = glm::radians(90.0f);

    loadCubeMap("../media/texture/cubemap_night/night");
}

void SceneRefractCube::loadCubeMap( const char * baseFileName )
{
    glActiveTexture(GL_TEXTURE0);

    Texture::loadCubeMap(baseFileName);

#ifdef __APPLE__
    prog.setUniform("CubeMapTex", 0);
#endif
}

void SceneRefractCube::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;

  angle += rotSpeed * deltaT;
  if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneRefractCube::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3( 7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    prog.setUniform("WorldCameraPosition", cameraPos);
    //view = glm::lookAt(vec3(0.0f,2.0f,0.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,0.0f,1.0f));

    prog.setUniform("DrawSkyBox", true);
    model = mat4(1.0f);
    setMatrices();
    sky.render();
    prog.setUniform("DrawSkyBox", false);

    prog.setUniform("Material.Eta", 0.94f);
    prog.setUniform("Material.ReflectionFactor", 0.1f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-1.0f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot.render();
}

void SceneRefractCube::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelMatrix", model);
    prog.setUniform("MVP", projection * mv);
}

void SceneRefractCube::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(50.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneRefractCube::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
    prog.compileShader("shader/cubemap_refract_41.vs");
    prog.compileShader("shader/cubemap_refract_41.fs");
#else
    prog.compileShader("shader/cubemap_refract.vs");
    prog.compileShader("shader/cubemap_refract.fs");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
