#include "sceneparticles.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::endl;
using std::cerr;

#include "bmpreader.h"

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneParticles::SceneParticles() : width(800), height(600), time(0) {}

void SceneParticles::initScene()
{
    compileAndLinkShader();

    glClearColor(0.1f,0.1f,0.1f,1.0f);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the point size
    glPointSize(10.0f);

	angle = glm::half_pi<float>();

	// Generate our vertex buffers
	initBuffers();

    // The particle texture
    const char * texName = "../media/texture/bluewater.bmp";
    glActiveTexture(GL_TEXTURE0);
    BMPReader::loadTex(texName);

    prog.setUniform("ParticleTex", 0);
    prog.setUniform("ParticleLifetime", 3.5f);
    prog.setUniform("Gravity", vec3(0.0f,-0.2f,0.0f));
}


void SceneParticles::initBuffers()
{
    nParticles = 8000;

    // Generate the buffers
    glGenBuffers(1, &initVel);   // Initial velocity buffer
    glGenBuffers(1, &startTime); // Start time buffer

    // Allocate space for all buffers
    int size = nParticles * 3 * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

    // Fill the first velocity buffer with random velocities
    vec3 v(0.0f);
    float velocity, theta, phi;
    GLfloat *data = new GLfloat[nParticles * 3];
    for (unsigned int i = 0; i < nParticles; i++ ) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, randFloat());
		phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);

        velocity = glm::mix(1.25f,1.5f,randFloat());
        v = glm::normalize(v) * velocity;

        data[3*i]   = v.x;
        data[3*i+1] = v.y;
        data[3*i+2] = v.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER,initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    // Fill the start time buffer
    delete [] data;
    data = new GLfloat[nParticles];
    float time = 0.0f;
    float rate = 0.00075f;
    for( unsigned int i = 0; i < nParticles; i++ ) {
        data[i] = time;
        time += rate;
    }
    glBindBuffer(GL_ARRAY_BUFFER,startTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    delete [] data;

    glGenVertexArrays(1, &particles);
	glBindVertexArray(particles);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

float SceneParticles::randFloat() {
    return ((float)rand() / RAND_MAX);
}

void SceneParticles::update( float t )
{
    time = t;
}

void SceneParticles::render()
{
	prog.setUniform("Time", time);
    glClear(GL_COLOR_BUFFER_BIT );
	view = glm::lookAt(vec3(3.0f * cos(angle),1.5f,3.0f * sin(angle)), vec3(0.0f,1.5f,0.0f), vec3(0.0f,1.0f,0.0f));
    model = mat4(1.0f);
    setMatrices();

	glBindVertexArray(particles);
	glDrawArrays(GL_POINTS,0,nParticles);
}

void SceneParticles::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("MVP", projection * mv);
}

void SceneParticles::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneParticles::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/particles.vs");
		prog.compileShader("shader/particles.fs");

    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
