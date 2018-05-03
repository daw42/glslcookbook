#include "scenealphatest.h"
#include "texture.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::mat4;

SceneAlphaTest::SceneAlphaTest() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 2.0f),
                                   teapot(14, glm::mat4(1.0f))
{ }

void SceneAlphaTest::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

    prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f) );

    // Load cement texture file
    glActiveTexture(GL_TEXTURE0);
    Texture::loadTexture("../media/texture/cement.jpg");

    // Load moss texture file
    glActiveTexture(GL_TEXTURE1);
    Texture::loadTexture("../media/texture/moss.png");

#ifdef __APPLE__
  prog.setUniform("BaseTex", 0);
  prog.setUniform("AlphaTex", 1);
#endif

}

void SceneAlphaTest::update( float t )
{
    float deltaT = t - tPrev;
    if(tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;

    if( this->m_animate ) {
	angle += rotSpeed * deltaT;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
    }
}

void SceneAlphaTest::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3(6.0f * cos(angle), 0.25f, 6.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));

    prog.setUniform("Light.Position", glm::vec4(0.0f,0.0f,0.0f,1.0f) );
    prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-1.5f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot.render();
}

void SceneAlphaTest::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneAlphaTest::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneAlphaTest::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
    prog.compileShader("shader/alphatest_41.vs");
    prog.compileShader("shader/alphatest_41.fs");
#else
    prog.compileShader("shader/alphatest.vs");
    prog.compileShader("shader/alphatest.fs");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
