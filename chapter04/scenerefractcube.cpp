#include "scenerefractcube.h"

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

SceneRefractCube::SceneRefractCube() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f) { }

void SceneRefractCube::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(14, mat4(1.0f));
    sky = new SkyBox();

    projection = mat4(1.0f);

    angle = glm::radians(90.0f);

    loadCubeMap("../media/texture/cubemap_night/night");
}

void SceneRefractCube::loadCubeMap( const char * baseFileName )
{
    glActiveTexture(GL_TEXTURE0);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    GLuint targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    GLint w, h;
#ifndef __APPLE__
    // Allocate immutable storage for the cube map texture
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 256, 256);
#endif

    // Load each cube-map face
    for( int i = 0; i < 6; i++ ) {
    	string texName = string(baseFileName) + "_" + suffixes[i] + ".tga";
    	GLubyte * data = TGAIO::read(texName.c_str(), w, h);
#ifdef __APPLE__
      glTexImage2D(targets[i], 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
#else
    	glTexSubImage2D(targets[i], 0, 0, 0, w, h,
    		    GL_RGBA, GL_UNSIGNED_BYTE, data);
#endif
    	delete [] data;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
    sky->render();
    prog.setUniform("DrawSkyBox", false);

    prog.setUniform("Material.Eta", 0.94f);
    prog.setUniform("Material.ReflectionFactor", 0.1f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-1.0f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
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
