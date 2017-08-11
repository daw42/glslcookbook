#include "scenetonemap.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"

#include <iostream>
using std::endl;
using std::cerr;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

SceneToneMap::SceneToneMap() : width(800), height(600), angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f) { }

void SceneToneMap::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(20.0f, 10.0f, 1, 1);
    teapot = new VBOTeapot(14, mat4(1.0));
    sphere = new VBOSphere(2.0f, 50, 50);

	angle = glm::pi<float>() / 2.0f;

    // Array for quad
    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
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
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(2);  // Texture coordinates

    glBindVertexArray(0);

    setupFBO();

    // Set up the subroutine indexes
    GLuint programHandle = prog.getHandle();
    pass1Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass1");
    pass2Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass2");

    vec3 intense = vec3(0.6f);
    prog.setUniform("Lights[0].Intensity", intense );
    prog.setUniform("Lights[1].Intensity", intense );
    prog.setUniform("Lights[2].Intensity", intense );

#ifdef __APPLE__
    prog.setUniform("HdrTex", 0);
#endif

}

void SceneToneMap::setupFBO()
{
    GLuint depthBuf;

    // Create and bind the FBO
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    // The depth buffer
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // The HDR color buffer
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &hdrTex);
    glBindTexture(GL_TEXTURE_2D, hdrTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Attach the images to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTex, 0);

    GLenum drawBuffers[] = {GL_NONE, GL_COLOR_ATTACHMENT0};
    glDrawBuffers(2, drawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneToneMap::update( float t )
{
}

void SceneToneMap::render()
{
    pass1();

    computeLogAveLuminance();

    pass2();
}

void SceneToneMap::pass1()
{
  glClearColor(0.5f,0.5f,0.5f,1.0f);
  glViewport(0,0,width,height);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass1Index);

    view = glm::lookAt(vec3(2.0f, 0.0f, 14.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width/height, 0.3f, 100.0f);

    drawScene();

    glFinish();
}

void SceneToneMap::computeLogAveLuminance()
{
  GLfloat *texData = new GLfloat[width*height*3];
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, hdrTex);
   glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData);
   float sum = 0.0f;
   for( int i = 0; i < width * height; i++ ) {
     float lum = glm::dot(vec3(texData[i*3+0], texData[i*3+1], texData[i*3+2]),
         vec3(0.2126f, 0.7152f, 0.0722f) );
     sum += logf( lum + 0.00001f );
   }

   prog.setUniform( "AveLum", expf( sum / (width*height) ) );
   //printf("(%f)\n", exp( sum / (width*height) ) );
   delete [] texData;
}

void SceneToneMap::pass2()
{
    // Revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass2Index);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    view = mat4(1.0);
    model = mat4(1.0);
    projection = mat4(1.0);
    setMatrices();

    // Render the quad
    glBindVertexArray(quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneToneMap::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneToneMap::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneToneMap::compileAndLinkShader()
{
    try {
#ifdef __APPLE__
        prog.compileShader("shader/tonemap_41.vs");
        prog.compileShader("shader/tonemap_41.fs");
#else
        prog.compileShader("shader/tonemap.vs");
    	prog.compileShader("shader/tonemap.fs");
#endif
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}

void SceneToneMap::drawScene()
{
    vec3 intense = vec3(1.0f);
    prog.setUniform("Lights[0].Intensity", intense );
    prog.setUniform("Lights[1].Intensity", intense );
    prog.setUniform("Lights[2].Intensity", intense );

    vec4 lightPos = vec4(0.0f, 4.0f, 2.5f, 1.0f);
    lightPos.x = -7.0f;
    prog.setUniform("Lights[0].Position", view * lightPos );
    lightPos.x = 0.0f;
    prog.setUniform("Lights[1].Position", view * lightPos );
    lightPos.x = 7.0f;
    prog.setUniform("Lights[2].Position", view * lightPos );

    prog.setUniform("Material.Kd", 0.9f, 0.3f, 0.2f);
    prog.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Shine", 100.0f);

    // The backdrop plane
    model = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f,0.0f,0.0f) );
    setMatrices();
    plane->render();

    // The bottom plane
    model = glm::translate(mat4(1.0f), vec3(0.0f,-5.0f,0.0f));
    setMatrices();
    plane->render();

    // Top plane
    model = glm::translate(mat4(1.0f), vec3(0.0f,5.0f,0.0f));
    model = glm::rotate( model, glm::radians(180.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    plane->render();

    prog.setUniform("Material.Kd", vec3(0.4f, 0.9f, 0.4f));
    model = glm::translate(mat4(1.0f), vec3(-3.0f,-3.0f,2.0f));
    setMatrices();
    sphere->render();

    prog.setUniform("Material.Kd", vec3(0.4f, 0.4f, 0.9f));
    model = glm::translate(mat4(1.0f), vec3(3.0f,-5.0f,1.5f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    //model = glm::scale(model, vec3(0.f));
    setMatrices();
    teapot->render();
}
