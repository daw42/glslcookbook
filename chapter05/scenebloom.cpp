#include "scenebloom.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneBloom::SceneBloom() : width(800), height(600), angle(0.0f), tPrev(0.0f), rotSpeed(PI/4.0)
{}

void SceneBloom::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    teapot = new VBOTeapot(14, mat4(1.0f));
    float c = 1.5f;
    torus = new VBOTorus(0.7f * c, 0.3f * c, 50,50);

    projection = mat4(1.0f);

    angle = PI / 2.0;

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
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(2);  // Texture coordinates

    glBindVertexArray(0);

    // Set up the subroutine indexes
    GLuint programHandle = prog.getHandle();
    pass1Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass1");
    pass2Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass2");
    pass3Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass3");
    pass4Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "pass4");

    prog.setUniform("Width", 800);
    prog.setUniform("Height", 600);
    prog.setUniform("RenderTex", 0);
    prog.setUniform("BlurTex", 1);
    prog.setUniform("LumThresh", 0.75f);
    prog.setUniform("Light[0].Intensity", vec3(0.7f,0.7f,0.7f) );
    prog.setUniform("Light[1].Intensity", vec3(0.7f,0.7f,0.7f) );

    char uniName[100];
    float weights[10], sum, sigma2 = 25.0f;

    // Compute and sum the weights
    weights[0] = gauss(0,sigma2);
    sum = weights[0];
    for( int i = 1; i < 10; i++ ) {
        weights[i] = gauss(i, sigma2);
        sum += 2 * weights[i];
    }

    // Normalize the weights and set the uniform
    for( int i = 0; i < 10; i++ ) {
        sprintf(uniName, "Weight[%d]", i);
        float val = weights[i] / sum;
        prog.setUniform(uniName, val);
    }
}

void SceneBloom::setupFBO() {
    // Generate and bind the framebuffer
    glGenFramebuffers(1, &renderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

    // Create the texture object
    glGenTextures(1, &renderTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

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

    // Generate and bind the framebuffer
    glGenFramebuffers(1, &fbo1);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);

    // Create the texture object
    glGenTextures(1, &tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    // Set the targets for the fragment output variables
    glDrawBuffers(1, drawBuffers);

    // Generate and bind the framebuffer
    glGenFramebuffers(1, &fbo2);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo2);

    // Create the texture object
    glGenTextures(1, &tex2);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2, 0);

    // Set the targets for the fragment output variables
    glDrawBuffers(1, drawBuffers);

    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneBloom::update( float t )
{
	float deltaT = t - tPrev;
	if(tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;

    angle += rotSpeed * deltaT;
    if( angle > TWOPI_F) angle -= TWOPI_F;
}

void SceneBloom::render()
{
    pass1();
    pass2();
    pass3();
    pass4();
}

void SceneBloom::pass1()
{
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass1Index);

    view = glm::lookAt(vec3(10.0f * cos(angle),3.0f,10.0f * sin(angle)), vec3(-1.0f,1.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = glm::perspective(50.0f, (float)width/height, 0.3f, 100.0f);

    prog.setUniform("Light[0].Position", view * vec4(-10.0f,10.0f,10.0f,1.0f) );
    prog.setUniform("Light[1].Position", view * vec4(10.0f,10.0f,10.0f,1.0f) );
    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.2f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,-.5f,0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Material.Kd", 0.4f, 0.5f, 0.6f);
    prog.setUniform("Material.Ks", 0.6f, 0.6f, 0.6f);
    prog.setUniform("Material.Ka", 0.02f, 0.02f, 0.02f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,-0.75f,0.0f));
    setMatrices();
    plane->render();

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.2f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(-2.5f,1.0f,4.0f));
    model *= glm::rotate(-60.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    torus->render();

    glFinish();
}

void SceneBloom::pass2()
{
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass2Index);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
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

void SceneBloom::pass3()
{
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass3Index);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo2);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneBloom::pass4()
{
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass4Index);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void SceneBloom::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneBloom::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(60.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneBloom::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/bloom.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/bloom.fs",GLSLShader::FRAGMENT))
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

float SceneBloom::gauss(float x, float sigma2 )
{
    double coeff = 1.0 / (2.0 * PI * sigma2);
    double expon = -(x*x) / (2.0 * sigma2);
    return (float) (coeff*exp(expon));
}
