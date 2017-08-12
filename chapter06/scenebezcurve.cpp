#include "scenebezcurve.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::endl;
using std::cerr;

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneBezCurve::SceneBezCurve() {}

void SceneBezCurve::initScene()
{
    compileAndLinkShader();
    glEnable(GL_DEPTH_TEST);

    float c = 3.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);
    glPointSize(10.0f);

    // Set up patch VBO
    float v[] = {-1.0f, -1.0f, -0.5f, 1.0f, 0.5f, -1.0f, 1.0f, 1.0f};

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

    // Segments and strips may be inverted on NVIDIA
    prog.use();
    prog.setUniform("NumSegments", 50);
    prog.setUniform("NumStrips", 1);
    prog.setUniform("LineColor", vec4(1.0f,1.0f,0.5f,1.0f));

    solidProg.use();
    solidProg.setUniform("Color", vec4(0.5f,1.0f,1.0f,1.0f));
}

void SceneBezCurve::update( float t ) {}

void SceneBezCurve::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(0.0f ,0.0f,1.5f);
    view = glm::lookAt(cameraPos,
                       vec3(0.0f,0.0f,0.0f),
                       vec3(0.0f,1.0f,0.0f));

    model = mat4(1.0f);

    glBindVertexArray(vaoHandle);
	setMatrices();

	// Draw the curve
    prog.use();
    glDrawArrays(GL_PATCHES, 0, 4);

	// Draw the control points
    solidProg.use();
    glDrawArrays(GL_POINTS, 0, 4);

    glFinish();
}

void SceneBezCurve::setMatrices()
{
    mat4 mv = view * model;
    prog.use();
    prog.setUniform("MVP", projection * mv);
    solidProg.use();
    solidProg.setUniform("MVP", projection * mv);
}

void SceneBezCurve::resize(int w, int h)
{
    glViewport(0,0,w,h);

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4( vec4(w2,0.0f,0.0f,0.0f),
                     vec4(0.0f,h2,0.0f,0.0f),
                     vec4(0.0f,0.0f,1.0f,0.0f),
                     vec4(w2+0, h2+0, 0.0f, 1.0f));
}

void SceneBezCurve::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/bezcurve.vs");
		prog.compileShader("shader/bezcurve.fs");
		prog.compileShader("shader/bezcurve.tes");
		prog.compileShader("shader/bezcurve.tcs");
    	prog.link();
    	prog.use();

    	solidProg.compileShader("shader/solid.vs");
    	solidProg.compileShader("shader/solid.fs");
    	solidProg.link();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
