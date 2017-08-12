#include "scenedecay.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;
#include "glutils.h"
#include "noisetex.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

#include<iostream>

SceneDecay::SceneDecay() : width(800), height(600) { }

void SceneDecay::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    prog.setUniform("NoiseTex", 0);

    GLuint noiseTex = NoiseTex::generate2DTex(10.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);

    teapot = new VBOTeapot(14, mat4(1.0f));

    prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f));
    lightPos = vec4(0.0f,0.0f,0.0f,1.0f);
    prog.setUniform("LowThreshold", 0.45f);
    prog.setUniform("HighThreshold", 0.65f);
}

void SceneDecay::update( float t ) { }

void SceneDecay::render()
{
    prog.setUniform("Light.Position", lightPos);

    vec3 cameraPos = 0.85f * vec3(0.0f,5.0f,6.0f);
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,-3.0f), vec3(0.0f,1.0f,0.0f));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glFinish();
}

void SceneDecay::drawScene()
{
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-15.0f), vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();

    prog.setUniform("Material.Kd", 0.7f, 0.5f, 0.3f);
    prog.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Shininess", 100.0f);

    teapot->render();
}

void SceneDecay::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneDecay::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(55.0f), (float)width/height, 0.3f, 100.0f);
}

void SceneDecay::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/decay.vs");
		prog.compileShader("shader/decay.fs");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
