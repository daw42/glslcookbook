#include "scenebasic_attrib.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;
#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

SceneBasic_Attrib::SceneBasic_Attrib() : width(0), height(0) { }

void SceneBasic_Attrib::initScene()
{
#ifndef __APPLE__
	glDebugMessageCallback(GLUtils::debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
		GL_DEBUG_SEVERITY_NOTIFICATION, -1 , "Start debugging");
#endif

	compileShaderProgram();

	std::cout << std::endl;

	prog.printActiveAttribs();

    /////////////////// Create the VBO ////////////////////
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f };
    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };

    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color

#ifdef __APPLE__
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
#else
		glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat)*3);
		glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat)*3);

		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(0, 0);
		glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	  glVertexAttribBinding(1, 1);
#endif

    glBindVertexArray(0);
}

void SceneBasic_Attrib::compileShaderProgram()
{
	try {
		prog.compileShader("shader/basic.vert" );
		prog.compileShader("shader/basic.frag" );
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Attrib::update( float t ) { }

void SceneBasic_Attrib::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );
		glBindVertexArray(0);
}

void SceneBasic_Attrib::resize(int w, int h)
{
		width = w;
		height = h;
    glViewport(0,0,w,h);
}
