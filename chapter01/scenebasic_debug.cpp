#include "scenebasic_debug.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;
#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

SceneBasic_Debug::SceneBasic_Debug() : width(0), height(0) { }

void SceneBasic_Debug::initScene()
{
	// Not necessary when we have a debug context.
	//glEnable(GL_DEBUG_OUTPUT);
#ifdef __APPLE__
  std::cerr << "This example requires OpenGL 4.3 or above." << std::endl;
	exit(1);
#endif

	glDebugMessageCallback(GLUtils::debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
		GL_DEBUG_SEVERITY_NOTIFICATION, -1 , "Start debugging");

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

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
}

void SceneBasic_Debug::compileShaderProgram()
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

void SceneBasic_Debug::update( float t ) { }

void SceneBasic_Debug::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );
}

void SceneBasic_Debug::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
