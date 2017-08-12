#include "scenequadtess.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneQuadTess::SceneQuadTess() { }

void SceneQuadTess::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    float c = 3.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);

    ///////////// Uniforms ////////////////////
    prog.setUniform("Inner", 4);
    prog.setUniform("Outer", 4);
    prog.setUniform("LineWidth", 1.5f);
    prog.setUniform("LineColor", vec4(0.05f,0.0f,0.05f,1.0f));
    prog.setUniform("QuadColor", vec4(1.0f,1.0f,1.0f,1.0f));
    /////////////////////////////////////////////

    // Set up patch VBO
    float v[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};

    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), v, GL_STATIC_DRAW);

    // Set up patch VAO
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Set the number of vertices per patch.  IMPORTANT!!
    glPatchParameteri( GL_PATCH_VERTICES, 4);

    GLint maxVerts;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxVerts);
    printf("Max patch vertices: %d\n", maxVerts);
}


void SceneQuadTess::update( float t ) {}

void SceneQuadTess::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(0.0f,0.0f,1.5f);
    view = glm::lookAt(cameraPos,
                       vec3(0.0f,0.0f,0.0f),
                       vec3(0.0f,1.0f,0.0f));

    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_PATCHES, 0, 4);

    glFinish();
}

void SceneQuadTess::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ViewportMatrix", viewport);
}

void SceneQuadTess::resize(int w, int h)
{
    glViewport(0,0,w,h);

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4( vec4(w2,0.0f,0.0f,0.0f),
                     vec4(0.0f,h2,0.0f,0.0f),
                     vec4(0.0f,0.0f,1.0f,0.0f),
                     vec4(w2+0, h2+0, 0.0f, 1.0f));
}

void SceneQuadTess::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/quadtess.vs");
		prog.compileShader("shader/quadtess.fs");
		prog.compileShader("shader/quadtess.gs");
		prog.compileShader("shader/quadtess.tes");
		prog.compileShader("shader/quadtess.tcs");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
