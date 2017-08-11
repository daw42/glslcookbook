#include "scenehdrbloom.h"

#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "glutils.h"

#include <iostream>
using std::endl;
using std::cerr;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneHdrBloom::SceneHdrBloom() : width(800), height(600), angle(0.0f), tPrev(0.0f),
  bloomBufWidth(width/8), bloomBufHeight(height/8)
{}

void SceneHdrBloom::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(20.0f, 10.0f, 1, 1);
    teapot = new VBOTeapot(14, mat4(1.0));
    sphere = new VBOSphere(2.0f, 50, 50);

    vec3 intense = vec3(0.6f);
    prog.setUniform("Lights[0].Intensity", intense );
    prog.setUniform("Lights[1].Intensity", intense );
    prog.setUniform("Lights[2].Intensity", intense );

    projection = mat4(1.0f);

	angle = glm::pi<float>() / 2.0f;

    setupFBO();

    // Array for full-screen quad
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

    glGenVertexArrays( 1, &fsQuad );
    glBindVertexArray(fsQuad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(2);  // Texture coordinates

    glBindVertexArray(0);

    // Set up the subroutine indexes
    GLuint programHandle = prog.getHandle();
    pass1Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass1");
    pass2Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass2");
    pass3Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass3");
    pass4Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass4");
    pass5Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass5");

    prog.setUniform("LumThresh", 1.7f);

    float weights[10], sum, sigma2 = 25.0f;

    // Compute and sum the weights
    weights[0] = gauss(0,sigma2);
    sum = weights[0];
    for( int i = 1; i < 10; i++ ) {
        weights[i] = gauss(float(i), sigma2);
        sum += 2 * weights[i];
    }

    // Normalize the weights and set the uniform
    for( int i = 0; i < 10; i++ ) {
		std::stringstream uniName;
		uniName << "Weight[" << i << "]";
        float val = weights[i] / sum;
        prog.setUniform(uniName.str().c_str(), val);
    }

    // Set up two sampler objects for linear and nearest filtering
    GLuint samplers[2];
    glGenSamplers(2, samplers);
    linearSampler = samplers[0];
    nearestSampler = samplers[1];

    GLfloat border[] = {0.0f,0.0f,0.0f,0.0f};
    // Set up the nearest sampler
    glSamplerParameteri(nearestSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(nearestSampler, GL_TEXTURE_BORDER_COLOR, border);

    // Set up the linear sampler
    glSamplerParameteri(linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(linearSampler, GL_TEXTURE_BORDER_COLOR, border);

    // We want nearest sampling except for the last pass.
    glBindSampler(0, nearestSampler);
    glBindSampler(1, nearestSampler);
    glBindSampler(2, nearestSampler);

#ifdef __APPLE__
    prog.setUniform("HdrTex", 0);
    prog.setUniform("BlurTex1", 1);
    prog.setUniform("BlurTex2", 2);
#endif
}

void SceneHdrBloom::setupFBO() {
    // Generate and bind the framebuffer
    glGenFramebuffers(1, &hdrFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo);

    // Create the texture object
    glGenTextures(1, &hdrTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width, height);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTex, 0);

    // Create the depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // Bind the depth buffer to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthBuf);

    // Set the targets for the fragment output variables
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    // Create an FBO for the bright-pass filter and blur
    glGenFramebuffers(1, &blurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);

    // Create two texture objects to ping-pong for the bright-pass filter
    // and the two-pass blur
    glGenTextures(1, &tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, bloomBufWidth, bloomBufHeight);

    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &tex2);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, bloomBufWidth, bloomBufHeight);

    // Bind tex1 to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    glDrawBuffers(1, drawBuffers);

    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneHdrBloom::update( float t )
{
}

void SceneHdrBloom::render()
{
    pass1();
    glFlush();
    computeLogAveLuminance();
    pass2();
    glFlush();
    pass3();
    glFlush();
    pass4();
    glFlush();
    pass5();
}

void SceneHdrBloom::computeLogAveLuminance()
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
   delete [] texData;
}


void SceneHdrBloom::pass1()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glViewport(0,0,width,height);

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass1Index);

    view = glm::lookAt(vec3(2.0f, 0.0f, 14.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width/height, 0.3f, 100.0f);

    drawScene();

    glFinish();
}

void SceneHdrBloom::pass2()
{
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass2Index);

    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);

    // We're writing to tex1 this time
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    glViewport(0,0,bloomBufWidth, bloomBufHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    model = mat4(1.0f);
    view = mat4(1.0f);
    projection = mat4(1.0f);
    setMatrices();

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glFinish();
}

void SceneHdrBloom::pass3()
{
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass3Index);
    // We're writing to tex2 this time
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2, 0);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneHdrBloom::pass4()
{
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass4Index);
    // We're writing to tex1 this time
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneHdrBloom::pass5()
{
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass5Index);

    // Bind to the default framebuffer, this time we're going to
    // actually draw to the screen!
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,width,height);

    // In this pass, we're reading from tex1 (unit 1) and we want
    // linear sampling to get an extra blur
    glBindSampler(1, linearSampler);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Revert to nearest sampling
    glBindSampler(1, nearestSampler);
}

void SceneHdrBloom::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneHdrBloom::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneHdrBloom::compileAndLinkShader()
{
    try {
#ifdef __APPLE__
        prog.compileShader("shader/hdrbloom_41.vs");
        prog.compileShader("shader/hdrbloom_41.fs");
#else
        prog.compileShader("shader/hdrbloom.vs");
    	prog.compileShader("shader/hdrbloom.fs");
#endif
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}

float SceneHdrBloom::gauss(float x, float sigma2 )
{
	double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
    double expon = -(x*x) / (2.0 * sigma2);
    return (float) (coeff*exp(expon));
}

void SceneHdrBloom::drawScene()
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
    prog.setUniform("Material.Shine", 25.0f);

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

    // Sphere
    prog.setUniform("Material.Kd", vec3(0.4f, 0.9f, 0.4f));
    model = glm::translate(mat4(1.0f), vec3(-3.0f,-3.0f,2.0f));
    setMatrices();
    sphere->render();

    // Teapot
    prog.setUniform("Material.Kd", vec3(0.4f, 0.4f, 0.9f));
    model = glm::translate(mat4(1.0f), vec3(4.0f,-5.0f,1.5f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
}
