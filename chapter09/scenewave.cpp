#include "scenewave.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::endl;
using std::cerr;

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneWave::SceneWave() : width(800), height(600), time(0) {}

void SceneWave::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(13.0f, 10.0f, 200, 2);

    prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f) );

	angle = glm::half_pi<float>();
}

void SceneWave::update( float t )
{
    time = t;
}

void SceneWave::render()
{
    prog.setUniform("Time", time);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(vec3(10.0f * cos(angle),4.0f,10.0f * sin(angle)), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width/height, 0.3f, 100.0f);

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Shininess", 100.0f);
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-10.0f), vec3(0.0f,0.0f,1.0f));
    model = glm::rotate(model, glm::radians(50.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    plane->render();
}

void SceneWave::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneWave::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneWave::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/wave.vs",GLSLShader::VERTEX);
		prog.compileShader("shader/wave.fs",GLSLShader::FRAGMENT);

    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
