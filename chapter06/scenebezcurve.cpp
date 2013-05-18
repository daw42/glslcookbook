#include "scenebezcurve.h"

#include <cstdio>
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneBezCurve::SceneBezCurve() {}

void SceneBezCurve::initScene()
{
    compileAndLinkShader();
GLUtils::checkForOpenGLError(__FILE__,__LINE__);
    glClearColor(0.5f,0.5f,0.5f,1.0f);
GLUtils::checkForOpenGLError(__FILE__,__LINE__);
    glEnable(GL_DEPTH_TEST);

    float c = 3.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);
    glPointSize(10.0f);

    // Set up patch VBO
    float v[] = {-1.0f, -1.0f, -0.5f, 1.0f, 0.5f, -1.0f, 1.0f, 1.0f};

    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), v, GL_STATIC_DRAW);

    // Set up patch VAO
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Set the number of vertices per patch.  IMPORTANT!!
    glPatchParameteri( GL_PATCH_VERTICES, 4);
}

void SceneBezCurve::update( float t ) {}

void SceneBezCurve::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(0.0f ,0.0f,1.5f);
    view = glm::lookAt(cameraPos,
                       vec3(0.0f,0.0f,0.0f),
                       vec3(0.0f,1.0f,0.0f));

    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(vaoHandle);

    prog.use();
    prog.setUniform("NumSegments", 5);
    prog.setUniform("NumStrips", 1);
    prog.setUniform("LineColor", vec4(1.0f,1.0f,0.5f,1.0f));
    glDrawArrays(GL_PATCHES, 0, 4);

    solidProg.use();
    solidProg.setUniform("Color", vec4(0.5f,1.0f,1.0f,1.0f));
    glDrawArrays(GL_POINTS, 0, 4);

    glFinish();
}

void SceneBezCurve::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("MVP", projection * mv);
    solidProg.setUniform("MVP", projection * mv);
}

void SceneBezCurve::resize(int w, int h)
{
    glViewport(0,0,w,h);

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4( vec4(w2,0.0f,0.0f,0.0f),
                     vec4(0.0f,h2,0.0f,0.0f),
                     vec4(0.0f,0.0f,1.0f,0.0f),
                     vec4(w2+0, h2+0, 0.0f, 1.0f));
}

void SceneBezCurve::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/bezcurve.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/bezcurve.fs",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/bezcurve.tes",GLSLShader::TESS_EVALUATION))
    {
        printf("Tess evaluation shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/bezcurve.tcs",GLSLShader::TESS_CONTROL))
    {
        printf("Tess control shader failed to compile!\n%s",
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

    if( ! solidProg.compileShaderFromFile("shader/solid.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               solidProg.log().c_str());
        exit(1);
    }
    if( ! solidProg.compileShaderFromFile("shader/solid.fs",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               solidProg.log().c_str());
        exit(1);
    }
    if( ! solidProg.link() )
    {
        printf("Shader program failed to link!\n%s",
               solidProg.log().c_str());
        exit(1);
    }

}
