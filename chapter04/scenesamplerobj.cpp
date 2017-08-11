#include "scenesamplerobj.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::endl;
using std::cerr;

#include "tgaio.h"
#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneSamplerObj::SceneSamplerObj() { }

void SceneSamplerObj::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);
	  glClearColor( 0.9f, 0.9f, 0.9f, 1.0f );
    plane = new VBOPlane(10.0f, 10.0f, 1, 1);

    view = glm::lookAt(vec3(0.0f,0.1f,6.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    angle = 0.0;

    prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f) );

    // A simple 128x128 checkerboard texture
    GLint w = 128, h = 128, checkSize = 4;
    GLubyte *data = new GLubyte[w * h * 4];
    for( int r = 0; r < h; ++r )
    	for(int c = 0; c < w; ++c )
    	{
    		GLubyte color = 0;
    		if( ((c/checkSize) + (r/checkSize)) % 2 == 0 )
    			color = 0;
    		else
    			color = 255;
    		data[(r * w + c)*4 + 0] = color;
    		data[(r * w + c)*4 + 1] = color;
    		data[(r * w + c)*4 + 2] = color;
    		data[(r * w + c)*4 + 3] = 255;
    	}

	// Create the texture object
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
#ifdef __APPLE__
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#else
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
#endif
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete [] data;

    // Create some sampler objects
    GLuint samplers[2];
    glGenSamplers(2, samplers);
    linearSampler = samplers[0];
    nearestSampler = samplers[1];

    // Set up the nearest sampler
    glSamplerParameteri(nearestSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set up the linear sampler
    glSamplerParameteri(linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Bind texture object and sampler object to texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

#ifdef __APPLE__
    // Bind the sampler object to the same texture unit and set the sampler uniform
    prog.setUniform("Tex1", 0);
#endif
}

void SceneSamplerObj::update( float t ) {}

void SceneSamplerObj::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Light.Position", vec4(0.0f,20.0f,0.0f,1.0f) );
    prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    glm::mat4 rot = glm::rotate(glm::mat4(), glm::radians(10.0f), glm::vec3(1,0,0));

    model = glm::translate(rot, glm::vec3(-5.01f,0.f,0.f));
    setMatrices();
    glBindSampler(0, nearestSampler);
    plane->render();

	model = glm::translate(rot, glm::vec3(5.01f,0.f,0.f));
    setMatrices();
    glBindSampler(0, linearSampler);
    plane->render();
}

void SceneSamplerObj::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneSamplerObj::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneSamplerObj::compileAndLinkShader()
{
    try {
#ifdef __APPLE__
      prog.compileShader("shader/texture_41.vs");
    	prog.compileShader("shader/texture_41.fs");
#else
    	prog.compileShader("shader/texture.vs");
    	prog.compileShader("shader/texture.fs");
#endif
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
     	cerr << e.what() << endl;
     	exit( EXIT_FAILURE );
    }
}
