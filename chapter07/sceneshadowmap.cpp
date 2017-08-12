#include "sceneshadowmap.h"

#include <cstdio>

#include "glutils.h"

using glm::vec3;

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>

SceneShadowMap::SceneShadowMap() : tPrev(0), width(800), height(600), shadowMapWidth(512),
shadowMapHeight(512) {}

void SceneShadowMap::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

	angle = glm::quarter_pi<float>();

    teapot = new VBOTeapot(14, mat4(1.0f));
    plane = new VBOPlane(40.0f, 40.0f, 2, 2);
    float scale = 2.0f;
    torus = new VBOTorus(0.7f * scale,0.3f * scale,50,50);
    // Set up the framebuffer object
    setupFBO();

    GLuint programHandle = prog.getHandle();
    pass1Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "recordDepth");
    pass2Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "shadeWithShadow");

    shadowBias = mat4( vec4(0.5f,0.0f,0.0f,0.0f),
                        vec4(0.0f,0.5f,0.0f,0.0f),
                        vec4(0.0f,0.0f,0.5f,0.0f),
                        vec4(0.5f,0.5f,0.5f,1.0f)
                        );

    lightFrustum = new Frustum(Projection::PERSPECTIVE);
    float c = 1.65f;
    vec3 lightPos = vec3(0.0f,c * 5.25f, c * 7.5f);  // World coords
    lightFrustum->orient( lightPos, vec3(0.0f), vec3(0.0f,1.0f,0.0f));
    lightFrustum->setPerspective( 50.0f, 1.0f, 1.0f, 25.0f);
    lightPV = shadowBias * lightFrustum->getProjectionMatrix() * lightFrustum->getViewMatrix();

    prog.setUniform("Light.Intensity", vec3(0.85f));

    prog.setUniform("ShadowMap", 0);
}

void SceneShadowMap::spitOutDepthBuffer() {
    int size = shadowMapWidth * shadowMapHeight;
    float * buffer = new float[size];
    unsigned char * imgBuffer = new unsigned char[size * 4];

    glGetTexImage(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,GL_FLOAT,buffer);

    for( int i = 0; i < shadowMapHeight; i++ )
        for( int j = 0; j < shadowMapWidth; j++ )
        {
            int imgIdx = 4 * ((i*shadowMapWidth) + j);
            int bufIdx = ((shadowMapHeight - i - 1) * shadowMapWidth) + j;

            // This is just to make a more visible image.  Scale so that
            // the range (minVal, 1.0) maps to (0.0, 1.0).  This probably should
            // be tweaked for different light configurations.
            float minVal = 0.88f;
            float scale = (buffer[bufIdx] - minVal) / (1.0f - minVal);
            unsigned char val = (unsigned char)(scale * 255);
            imgBuffer[imgIdx] = val;
            imgBuffer[imgIdx+1] = val;
            imgBuffer[imgIdx+2] = val;
            imgBuffer[imgIdx+3] = 0xff;
        }

    delete [] buffer;
    delete [] imgBuffer;
    exit(1);
}

void SceneShadowMap::setupFBO()
{
    GLfloat border[] = {1.0f, 0.0f,0.0f,0.0f };
    // The depth buffer texture
    GLuint depthTex;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, shadowMapWidth, shadowMapHeight);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    // Assign the depth buffer texture to texture channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex);

    // Create and set up the FBO
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, depthTex, 0);

    GLenum drawBuffers[] = {GL_NONE};
    glDrawBuffers(1, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    } else {
        printf("Framebuffer is not complete.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void SceneShadowMap::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;

  angle += 0.2f * deltaT;
  if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneShadowMap::render()
{
    prog.use();
    // Pass 1 (shadow map generation)
    view = lightFrustum->getViewMatrix();
    projection = lightFrustum->getProjectionMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,shadowMapWidth,shadowMapHeight);
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass1Index);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.5f,10.0f);
    drawScene();
    glCullFace(GL_BACK);
    glFlush();
    //spitOutDepthBuffer(); // This is just used to get an image of the depth buffer

    // Pass 2 (render)
    float c = 1.0f;
    vec3 cameraPos(c * 11.5f * cos(angle),c * 7.0f,c * 11.5f * sin(angle));
    view = glm::lookAt(cameraPos,vec3(0.0f),vec3(0.0f,1.0f,0.0f));
    prog.setUniform("Light.Position", view * vec4(lightFrustum->getOrigin(),1.0f));
    projection = glm::perspective(glm::radians(50.0f), (float)width/height, 0.1f, 100.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,width,height);
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass2Index);
    drawScene();

    // Uncomment to draw the light's frustum
//    solidProg.use();
//    solidProg.setUniform("Color", vec4(1.0f,0.0f,0.0f,1.0f));
//    model = mat4(1.0f);
//    mat4 mv = view * model;
//    solidProg.setUniform("MVP", projection * mv);
//    lightFrustum->render();
//    glFinish();
}

void SceneShadowMap::drawScene()
{
    vec3 color = vec3(0.7f,0.5f,0.3f);
    prog.setUniform("Material.Ka", color * 0.05f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.9f,0.9f,0.9f));
    prog.setUniform("Material.Shininess", 150.0f);
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Material.Ka", color * 0.05f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.9f,0.9f,0.9f));
    prog.setUniform("Material.Shininess", 150.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,2.0f,5.0f));
    model = glm::rotate(model, glm::radians(-45.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    torus->render();

    prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    setMatrices();
    plane->render();
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-5.0f,5.0f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f),vec3(0.0f,0.0f,1.0f));
    setMatrices();
    plane->render();
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,5.0f,-5.0f));
    model = glm::rotate(model, glm::radians(90.0f),vec3(1.0f,0.0f,0.0f));
    setMatrices();
    plane->render();
    model = mat4(1.0f);
}

void SceneShadowMap::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ShadowMatrix", lightPV * model);
}

void SceneShadowMap::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneShadowMap::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/shadowmap.vs");
		prog.compileShader("shader/shadowmap.fs");
    	prog.link();
    	prog.use();

        // Used when rendering light frustum
    	//solidProg.compileShader("shader/solid.vs", GLSLShader::VERTEX);
    	//solidProg.compileShader("shader/solid.fs", GLSLShader::FRAGMENT);
    	//solidProg.link();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
