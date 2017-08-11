#include "scenespot.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"

using glm::vec3;

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>

SceneSpot::SceneSpot() : tPrev(0)
{
}

void SceneSpot::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    teapot = new VBOTeapot(14, glm::mat4(1.0f));
    torus = new VBOTorus(1.75f * 0.75f, 0.75f * 0.75f, 50, 50);

    view = glm::lookAt(vec3(5.0f,5.0f,7.5f), vec3(0.0f,0.75f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    angle = 0.0;

    prog.setUniform("Spot.intensity", vec3(0.9f,0.9f,0.9f) );
    prog.setUniform("Spot.exponent", 30.0f );
    prog.setUniform("Spot.cutoff", 15.0f );
}

void SceneSpot::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;

  angle += 0.25f * deltaT;
  if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneSpot::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
    prog.setUniform("Spot.position", view * lightPos);
    mat3 normalMatrix = mat3( vec3(view[0]), vec3(view[1]), vec3(view[2]) );
    prog.setUniform("Spot.direction", normalMatrix * vec3(-lightPos) );

    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
    prog.setUniform("Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,0.0f,-2.0f));
    model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
    prog.setUniform("Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(-1.0f,0.75f,3.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    torus->render();

    prog.setUniform("Kd", 0.7f, 0.7f, 0.7f);
    prog.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Shininess", 180.0f);

    model = mat4(1.0f);
    setMatrices();
    plane->render();
}

void SceneSpot::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneSpot::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneSpot::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/spot.vs");
		prog.compileShader("shader/spot.fs");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
