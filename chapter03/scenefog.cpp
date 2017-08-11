#include "scenefog.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"

#include <iostream>
using std::endl;
using std::cerr;

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneFog::SceneFog() : tPrev(0.0f)
{
}

void SceneFog::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    teapot = new VBOTeapot(14, glm::mat4(1.0f));
    torus = new VBOTorus(1.75f * 0.75f, 0.75f * 0.75f, 50, 50);

    view = glm::lookAt(vec3(0.0f,4.0f,6.0f), vec3(0.0f,2.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    angle = 0.0;

    prog.setUniform("Light.intensity", vec3(0.9f,0.9f,0.9f) );
    prog.setUniform("Fog.maxDist", 30.0f );
    prog.setUniform("Fog.minDist", 1.0f );
    prog.setUniform("Fog.color", vec3(0.5f,0.5f,0.5f) );
}

void SceneFog::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;

  angle += 1.0f * deltaT;
  if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneFog::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
    prog.setUniform("Light.position", view * lightPos);

    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
    prog.setUniform("Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Shininess", 180.0f);

    float dist = 0.0f;
    for( int i = 0 ; i < 4; i++ ) {
        model = mat4(1.0f);
        model = glm::translate(model, vec3(dist * 0.6f - 1.0f,0.0f,-dist));
        model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
        setMatrices();
        teapot->render();
        dist += 7.0f;
    }

    prog.setUniform("Kd", 0.7f, 0.7f, 0.7f);
    prog.setUniform("Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Shininess", 180.0f);

    model = mat4(1.0f);
    setMatrices();
    plane->render();
}

void SceneFog::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneFog::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneFog::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/fog.vs");
		prog.compileShader("shader/fog.fs");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
