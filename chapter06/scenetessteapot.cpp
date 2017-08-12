#include "scenetessteapot.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

SceneTessTeapot::SceneTessTeapot() :angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f) {}

void SceneTessTeapot::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

	angle = glm::pi<float>() / 3.0f;
    teapot = new VBOTeapotPatch();

    ///////////// Uniforms ////////////////////
    prog.setUniform("TessLevel", 4);
    prog.setUniform("LineWidth", 0.8f);
    prog.setUniform("LineColor", vec4(0.05f,0.0f,0.05f,1.0f));
    prog.setUniform("LightPosition", vec4(0.0f,0.0f,0.0f,1.0f));
    prog.setUniform("LightIntensity", vec3(1.0f,1.0f,1.0f));
    prog.setUniform("Kd", vec3(0.9f, 0.9f, 1.0f));
    /////////////////////////////////////////////

    glPatchParameteri(GL_PATCH_VERTICES, 16);
}


void SceneTessTeapot::update( float t )
{
	float deltaT = t - tPrev;
	if(tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;

    angle += rotSpeed * deltaT;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneTessTeapot::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(4.25f * cos(angle),3.0f,4.25f * sin(angle));
    view = glm::lookAt(cameraPos,
                       vec3(0.0f,0.0f,0.0f),
                       vec3(0.0f,1.0f,0.0f));

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-1.5f,0.0f));
    model = glm::rotate(model,glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
    glFinish();
}

void SceneTessTeapot::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ViewportMatrix", viewport);
}

void SceneTessTeapot::resize(int w, int h)
{
    glViewport(0,0,w,h);
    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4( vec4(w2,0.0f,0.0f,0.0f),
                     vec4(0.0f,h2,0.0f,0.0f),
                     vec4(0.0f,0.0f,1.0f,0.0f),
                     vec4(w2+0, h2+0, 0.0f, 1.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneTessTeapot::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/tessteapot.vs",GLSLShader::VERTEX);
		prog.compileShader("shader/tessteapot.fs",GLSLShader::FRAGMENT);
		prog.compileShader("shader/tessteapot.gs",GLSLShader::GEOMETRY);
		prog.compileShader("shader/tessteapot.tes",GLSLShader::TESS_EVALUATION);
		prog.compileShader("shader/tessteapot.tcs",GLSLShader::TESS_CONTROL);
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
