#include "scenetessteapotdepth.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneTessTeapotDepth::SceneTessTeapotDepth() { }

void SceneTessTeapotDepth::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapotPatch();

    ///////////// Uniforms ////////////////////
    prog.setUniform("MinTessLevel", 2);
    prog.setUniform("MaxTessLevel", 15);
    prog.setUniform("MaxDepth", 20.0f);
    prog.setUniform("MinDepth", 2.0f);
    prog.setUniform("LineWidth", 0.8f);
    prog.setUniform("LineColor", vec4(0.05f,0.0f,0.05f,1.0f));
    prog.setUniform("LightPosition", vec4(0.0f,0.0f,0.0f,1.0f));
    prog.setUniform("LightIntensity", vec3(1.0f,1.0f,1.0f));
    prog.setUniform("Kd", vec3(0.9f, 0.9f, 1.0f));
    /////////////////////////////////////////////

    glPatchParameteri(GL_PATCH_VERTICES, 16);
}


void SceneTessTeapotDepth::update( float t ) {}

void SceneTessTeapotDepth::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(0.0f ,1.0f,6.25f);
    view = glm::lookAt(cameraPos,
                       vec3(0.0f,0.0f,0.0f),
                       vec3(0.0f,1.0f,0.0f));

    model = mat4(1.0f);
    model = glm::translate(model, vec3(-2.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f,-1.5f,0.0f));
    model = glm::rotate(model,glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, 0.0f, -5.0f));
    model = glm::translate(model, vec3(0.0f,-1.5f,0.0f));
    model = glm::rotate(model,glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(7.0f, 0.0f, -10.0f));
    model = glm::translate(model, vec3(0.0f,-1.5f,0.0f));
    model = glm::rotate(model,glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(17.0f, 0.0f, -20.0f));
    model = glm::translate(model, vec3(0.0f,-1.5f,0.0f));
    model = glm::rotate(model,glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    glFinish();
}

void SceneTessTeapotDepth::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ViewportMatrix", viewport);
}

void SceneTessTeapotDepth::resize(int w, int h)
{
    glViewport(0,0,w,h);

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4( vec4(w2,0.0f,0.0f,0.0f),
                     vec4(0.0f,h2,0.0f,0.0f),
                     vec4(0.0f,0.0f,1.0f,0.0f),
                     vec4(w2+0, h2+0, 0.0f, 1.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 30.0f);
}

void SceneTessTeapotDepth::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/tessteapotdepth.vs",GLSLShader::VERTEX);
		prog.compileShader("shader/tessteapotdepth.fs",GLSLShader::FRAGMENT);
		prog.compileShader("shader/tessteapotdepth.gs",GLSLShader::GEOMETRY);
		prog.compileShader("shader/tessteapotdepth.tes",GLSLShader::TESS_EVALUATION);
		prog.compileShader("shader/tessteapotdepth.tcs",GLSLShader::TESS_CONTROL);
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
