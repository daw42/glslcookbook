#include "scenerendertotex.h"

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#include <cstdio>
#include <cstdlib>

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneRenderToTex::SceneRenderToTex() : angle(0.0f), tPrev(0.0f), rotSpeed(PI/2.0) { }

void SceneRenderToTex::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    cube = new VBOCube();

    projection = mat4(1.0f);

    teapot = new VBOTeapot(14, mat4(1.0f));

    angle = (float)( TO_RADIANS(140.0f) );

    prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f) );
    setupFBO();

    // One pixel white texture
    GLuint whiteTexHandle;
    GLubyte whiteTex[] = { 255, 255, 255, 255 };
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &whiteTexHandle);
    glBindTexture(GL_TEXTURE_2D,whiteTexHandle);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,whiteTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

void SceneRenderToTex::setupFBO() {
    // Generate and bind the framebuffer
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    // Create the texture object
    GLuint renderTex;
    glGenTextures(1, &renderTex);
    glActiveTexture(GL_TEXTURE0);  // Use texture unit 0
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

    // Create the depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);

    // Bind the depth buffer to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthBuf);

    // Set the targets for the fragment output variables
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( result == GL_FRAMEBUFFER_COMPLETE) {
		cout << "Framebuffer is complete" << endl;
	} else {
		cout << "Framebuffer error: " << result << endl;
	}

    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderToTex::update( float t )
{
	float deltaT = t - tPrev;
	if(tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;

    angle += rotSpeed * deltaT;
    if( angle > TWOPI_F) angle -= TWOPI_F;
}

void SceneRenderToTex::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    renderToTexture();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    renderScene();

	GLUtils::checkForOpenGLError(__FILE__,__LINE__);
}

void SceneRenderToTex::renderToTexture() {
    prog.setUniform("RenderTex", 1);
    glViewport(0,0,512,512);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(vec3(0.0f,0.0f,7.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = glm::perspective(60.0f, 1.0f, 0.3f, 100.0f);

    prog.setUniform("Light.Position", vec4(0.0f,0.0f,0.0f,1.0f) );
    prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-1.5f,0.0f));
    model = glm::rotate(model, -90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
}

void SceneRenderToTex::renderScene() {
    prog.setUniform("RenderTex", 0);
    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3(2.0f * cos(angle), 1.5f, 2.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));

    projection = glm::perspective(45.0f, (float)width/height, 0.3f, 100.0f);

    prog.setUniform("Light.Position", vec4(0.0f,0.0f,0.0f,1.0f) );
    prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);
    setMatrices();
    cube->render();
}

void SceneRenderToTex::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneRenderToTex::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(60.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneRenderToTex::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/rendertotex.vs",GLSLShader::VERTEX);
		prog.compileShader("shader/rendertotex.fs",GLSLShader::FRAGMENT);
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
