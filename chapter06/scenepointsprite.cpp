#include "scenepointsprite.h"

#include <cstdio>
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include <iostream>
using std::cerr;
using std::endl;

#include "bmpreader.h"

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

ScenePointSprite::ScenePointSprite() {}

void ScenePointSprite::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    numSprites = 50;
    locations = new float[numSprites * 3];
    srand( (unsigned int)time(0) );
    for( int i = 0; i < numSprites; i++ ) {
        vec3 p(((float)rand() / RAND_MAX * 2.0f) - 1.0f,
               ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
               ((float)rand() / RAND_MAX * 2.0f) - 1.0f);
        locations[i*3] = p.x;
        locations[i*3+1] = p.y;
        locations[i*3+2] = p.z;
    }

    // Set up the buffers
    GLuint handle;
    glGenBuffers(1, &handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, numSprites * 3 * sizeof(float), locations, GL_STATIC_DRAW);

    delete [] locations;

    // Set up the vertex array object
    glGenVertexArrays( 1, &sprites );
    glBindVertexArray(sprites);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindVertexArray(0);

    // Load texture file
    GLuint w, h;
    const char * texName = "../media/texture/flower.bmp";
    BMPReader::loadTex(texName, w, h);

    prog.setUniform("SpriteTex", 0);
    prog.setUniform("Size2", 0.15f);
}


void ScenePointSprite::update( float t ) { }

void ScenePointSprite::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(0.0f,0.0f,3.0f);
    view = glm::lookAt(cameraPos,
                       vec3(0.0f,0.0f,0.0f),
                       vec3(0.0f,1.0f,0.0f));

    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(sprites);
    glDrawArrays(GL_POINTS, 0, numSprites);

    glFinish();
}

void ScenePointSprite::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("ProjectionMatrix", projection);
}

void ScenePointSprite::resize(int w, int h)
{
    glViewport(0,0,w,h);
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void ScenePointSprite::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/pointsprite.vs");
		prog.compileShader("shader/pointsprite.fs");
		prog.compileShader("shader/pointsprite.gs");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
