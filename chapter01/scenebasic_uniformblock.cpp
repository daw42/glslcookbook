#include "scenebasic_uniformblock.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
using std::string;

#include <iostream>
using std::endl;
using std::cerr;

#include "glutils.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;

SceneBasic_UniformBlock::SceneBasic_UniformBlock() : width(0), height(0), angle(0.0f) { }

void SceneBasic_UniformBlock::initUniformBlockBuffer()
{
	GLuint programHandle = prog.getHandle();

    // Get the index of the uniform block
    GLuint blockIndex = glGetUniformBlockIndex(programHandle, "BlobSettings");

    // Allocate space for the buffer
    GLint blockSize;
    glGetActiveUniformBlockiv(programHandle, blockIndex,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    GLubyte * blockBuffer;
    blockBuffer = (GLubyte *) malloc(blockSize);

    // Query for the offsets of each block variable
    const GLchar *names[] = { "BlobSettings.InnerColor", "BlobSettings.OuterColor",
                              "BlobSettings.RadiusInner", "BlobSettings.RadiusOuter" };

    GLuint indices[4];
    glGetUniformIndices(programHandle, 4, names, indices);

    GLint offset[4];
    glGetActiveUniformsiv(programHandle, 4, indices, GL_UNIFORM_OFFSET, offset);

    // Store data within the buffer at the appropriate offsets
    GLfloat outerColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLfloat innerColor[] = {1.0f, 1.0f, 0.75f, 1.0f};
    GLfloat innerRadius = 0.25f, outerRadius = 0.45f;

    memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[2], &innerRadius, sizeof(GLfloat));
    memcpy(blockBuffer + offset[3], &outerRadius, sizeof(GLfloat));

    // Create the buffer object and copy the data
    GLuint uboHandle;
    glGenBuffers( 1, &uboHandle );
    glBindBuffer( GL_UNIFORM_BUFFER, uboHandle );
    glBufferData( GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW );

    // Bind the buffer object to the uniform block
    glBindBufferBase( GL_UNIFORM_BUFFER, 1, uboHandle );

#ifdef __APPLE__
    // We don't need this if we specify the binding within the shader (OpenGL 4.2 and above)
    glUniformBlockBinding(programHandle, blockIndex, 1);
#endif
}

void SceneBasic_UniformBlock::initScene()
{
    compile();

    std::cout << std::endl;
    initUniformBlockBuffer();
		prog.printActiveUniformBlocks();

    /////////////////// Create the VBO ////////////////////
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.8f,  0.8f, 0.0f,
         -0.8f, -0.8f, 0.0f,
          0.8f, 0.8f, 0.0f,
          -0.8f, 0.8f, 0.0f
     };
    float tcData[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint tcBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), tcData, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex texture coords

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneBasic_UniformBlock::compile()
{
	try {
#ifdef __APPLE__
		prog.compileShader("shader/basic_uniformblock_41.vert");
		prog.compileShader("shader/basic_uniformblock_41.frag");
#else
		prog.compileShader("shader/basic_uniformblock.vert");
		prog.compileShader("shader/basic_uniformblock.frag");
#endif
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_UniformBlock::update( float t )
{
    angle += 1.0f;
    if( angle >= 360.0f) angle -= 360.0f;
}

void SceneBasic_UniformBlock::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6 );
}

void SceneBasic_UniformBlock::resize(int w, int h)
{
		width = w;
		height = h;
    glViewport(0,0,w,h);
}
