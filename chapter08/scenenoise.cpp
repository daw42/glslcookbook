#include "scenenoise.h"

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

SceneNoise::SceneNoise() : width(800), height(600) { }

void SceneNoise::initScene()
{
    compileAndLinkShader();

    glClearColor(1.0f,1.0f,1.0f,1.0f);

    glEnable(GL_DEPTH_TEST);

    float c = 0.75f;
    projection = glm::ortho(c * -2.0f,c*2.0f,c *(-1.5f), c * 1.5f);

    // Array for quad
    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    float max = 2.0f;
    GLfloat tc[] = {
        0.0f, 0.0f, max, 0.0f, max, max,
        0.0f, 0.0f, max, max, 0.0f, max
    };

    // Set up the buffers
    unsigned int handle[2];
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    // Set up the vertex array object
    glGenVertexArrays( 1, &quad );
    glBindVertexArray(quad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(2);  // Texture coordinates

    glBindVertexArray(0);

    GLuint noiseTex = NoiseTex::generatePeriodic2DTex();
    //GLuint noiseTex = NoiseTex::generate2DTex();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);
}

void SceneNoise::update( float t ) { }

void SceneNoise::render()
{
    view = mat4(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glFinish();
}

void SceneNoise::drawScene()
{
    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneNoise::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("MVP", projection * mv);
}

void SceneNoise::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneNoise::compileAndLinkShader()
{
    try {
	prog.compileShader("shader/noisetex.vs");
	prog.compileShader("shader/noisetex.fs");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 	exit( EXIT_FAILURE );
    }
}
