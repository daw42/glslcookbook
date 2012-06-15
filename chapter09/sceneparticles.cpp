#include "sceneparticles.h"

#include <cstdio>
#include <cstdlib>

#include <glimg/glimg.h>

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneParticles::SceneParticles()
{
    width = 800;
    height = 600;
    time = 0.0f;
}

void SceneParticles::initScene()
{
    compileAndLinkShader();

    glClearColor(0.1f,0.1f,0.1f,1.0f);

//    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the point size
    glPointSize(10.0f);

    plane = new VBOPlane(13.0f, 10.0f, 200, 2);
    projection = mat4(1.0f);

    //prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f) );

    angle = (float)(PI / 2.0f);

	// Generate our vertex buffers
	initBuffers();

    // The particle texture
    const char * texName = "../media/texture/bluewater.png";
	try {
		glimg::ImageSet * imgSet;
		imgSet = glimg::loaders::stb::LoadFromFile(texName);
		const glimg::SingleImage &img = imgSet->GetImage(0);
		glimg::OpenGLPixelTransferParams params = glimg::GetUploadFormatType(img.GetFormat(), 0);
		glimg::Dimensions dims = img.GetDimensions();

		glPixelStorei(GL_UNPACK_ALIGNMENT, img.GetFormat().LineAlign());

		// Copy file to OpenGL
		glActiveTexture(GL_TEXTURE0);
		GLuint tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dims.width, dims.height, 0,
					 params.format, params.type, img.GetImageData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete imgSet;

	} catch( glimg::loaders::stb::StbLoaderException &e ) {
		fprintf(stderr, "Unable to load texture %s: %s\n", texName, e.what());
		exit(1);
	}

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

        theta = glm::mix(0.0f, (float)PI / 6.0f, randFloat());
        phi = glm::mix(0.0f, (float)TWOPI, randFloat());

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

    // Attach these to the torus's vertex array
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
//    prog.setUniform("ModelViewMatrix", mv);
//    prog.setUniform("NormalMatrix",
//                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneParticles::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(60.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneParticles::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/particles.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/particles.fs",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.link() )
    {
        printf("Shader program failed to link!\n%s",
               prog.log().c_str());
        exit(1);
    }

    prog.use();
}
