#include "scenebasic.h"

#include <cstdio>
#include <cstdlib>

#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;

#include "glutils.h"

SceneBasic::SceneBasic()
{
}

void SceneBasic::initScene()
{
    //////////////////////////////////////////////////////
    /////////// Vertex shader //////////////////////////
    //////////////////////////////////////////////////////

    GLchar * shaderCode;

    // Load contents of file into shaderCode here�
    ifstream inFile( "shader/basic.vert", ifstream::in );
    if (!inFile) {
        fprintf(stderr, "Error opening file: shader/basic.vert\n" );
        exit(1);
    }

    shaderCode = (char *)malloc(10000);
    int i = 0;
    while( inFile.good() ) {
        // NOTE: the last character read will be invalid
        int c = inFile.get();
        shaderCode[i++] = c;
    }
    inFile.close();
    shaderCode[--i] = '\0';     // null last character filled (invalid)
    ////////////////////////////////////////////

    // Create the shader object
    GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
    if (0 == vertShader) {
      fprintf(stderr, "Error creating vertex shader.\n");
      exit(1);
    }

    // Load the source code into the shader object
    const GLchar* codeArray[] = {shaderCode};
    glShaderSource(vertShader, 1, codeArray, NULL);
    free(shaderCode); // can be removed from book.

    // Compile the shader
    glCompileShader( vertShader );

    // Check compilation status
    GLint result;
    glGetShaderiv( vertShader, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ) {
       fprintf( stderr, "Vertex shader compilation failed!\n" );

       GLint logLen;
       glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &logLen );

       if (logLen > 0) {
           char * log = (char *)malloc(logLen);

           GLsizei written;
           glGetShaderInfoLog(vertShader, logLen, &written, log);

           fprintf(stderr, "Shader log: \n%s", log);

           free(log);
       }
    }

    //////////////////////////////////////////////////////
    /////////// Fragment shader //////////////////////////
    //////////////////////////////////////////////////////

    // Load contents of file into shaderCode here�
    ifstream fragFile( "shader/basic.frag", ifstream::in );
    if (!fragFile) {
        fprintf(stderr, "Error opening file: shader/basic.frag\n" );
        exit(1);
    }

    shaderCode = (char *)malloc(10000);
    i = 0;
    while (fragFile.good()) {
        int c = fragFile.get();
        shaderCode[i++] = c;
    }
    inFile.close();
    shaderCode[--i] = '\0';
    ////////////////////////////////////////////

    // Create the shader object
    GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
    if (0 == fragShader) {
      fprintf(stderr, "Error creating fragment shader.\n");
      exit(1);
    }

    // Load the source code into the shader object
    //const GLchar* codeArray[] = {shaderCode};
    codeArray[0] = shaderCode;
    glShaderSource( fragShader, 1, codeArray, NULL );
    free(shaderCode); // can be removed from book.

    // Compile the shader
    glCompileShader( fragShader );

    // Check compilation status
    //GLint result;
    glGetShaderiv( fragShader, GL_COMPILE_STATUS, &result );
    if (GL_FALSE == result) {
       fprintf( stderr, "Fragment shader compilation failed!\n" );

       GLint logLen;
       glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &logLen );

       if (logLen > 0) {
           char * log = (char *)malloc(logLen);

           GLsizei written;
           glGetShaderInfoLog(fragShader, logLen, &written, log);

           fprintf(stderr, "Shader log: \n%s", log);

           free(log);
       }
    }

    linkMe(vertShader, fragShader);

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

void SceneBasic::linkMe(GLint vertShader, GLint fragShader)
{
    // Create the program object
    GLuint programHandle = glCreateProgram();
    if(0 == programHandle) {
        fprintf(stderr, "Error creating program object.\n");
        exit(1);
    }

    // Bind index 0 to the shader input variable "VertexPosition"
    glBindAttribLocation(programHandle, 0, "VertexPosition");
    // Bind index 1 to the shader input variable "VertexColor"
    glBindAttribLocation(programHandle, 1, "VertexColor");

    // Attach the shaders to the program object
    glAttachShader( programHandle, vertShader );
    glAttachShader( programHandle, fragShader );

    // Link the program
    glLinkProgram( programHandle );

    // Check for successful linking
    GLint status;
    glGetProgramiv( programHandle, GL_LINK_STATUS, &status );
    if (GL_FALSE == status) {

        fprintf( stderr, "Failed to link shader program!\n" );

        GLint logLen;
        glGetProgramiv( programHandle, GL_INFO_LOG_LENGTH, &logLen );

        if (logLen > 0) {
            char * log = (char *)malloc(logLen);

            GLsizei written;
            glGetProgramInfoLog(programHandle, logLen, &written, log);

            fprintf(stderr, "Program log: \n%s", log);

            free(log);
        }
    } else {
        glUseProgram( programHandle );
    }
}

void SceneBasic::update( float t )
{

}

void SceneBasic::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );
}

void SceneBasic::resize(int w, int h)
{
    glViewport(0,0,w,h);
}
