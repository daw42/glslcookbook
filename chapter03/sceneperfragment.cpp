#include "sceneperfragment.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"

using glm::vec3;

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>

ScenePerFragment::ScenePerFragment() : tPrev(0)
{
}

void ScenePerFragment::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    teapot = new VBOTeapot(14, glm::mat4(1.0f));
    torus = new VBOTorus(0.7f * 2, 0.3f * 2, 50, 50);

    view = glm::lookAt(vec3(0.0f,3.0f,5.0f), vec3(0.0f,0.75f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    angle = 0.957283f;

    prog.setUniform("LightIntensity", vec3(0.9f,0.9f,0.9f) );
}

void ScenePerFragment::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;

  angle += 0.25f * deltaT;
  if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void ScenePerFragment::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("LightPosition", view * vec4(10.0f * cos(angle), 3.0f, 10.0f * sin(angle), 1.0f) );
    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,0.0f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Kd", 0.7f, 0.7f, 0.7f);
    prog.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-0.45f,0.0f));
    setMatrices();
    plane->render();
}

void ScenePerFragment::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void ScenePerFragment::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void ScenePerFragment::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/perfrag.vs",GLSLShader::VERTEX);
		prog.compileShader("shader/perfrag.fs",GLSLShader::FRAGMENT);
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
