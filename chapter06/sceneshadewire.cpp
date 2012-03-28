#include "sceneshadewire.h"

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

SceneShadeWire::SceneShadeWire()
{
    width = 800;
    height = 600;
}

void SceneShadeWire::initScene()
{
    compileAndLinkShader();
    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    ogre = new VBOMesh("../media/bs_ears.obj");
    float c = 1.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);

    angle = (float)(PI / 2.0);    
	///////////// Uniforms ////////////////////
    prog.setUniform("Line.Width", 0.75f);
	prog.setUniform("Line.Color", vec4(0.05f,0.0f,0.05f,1.0f));
	prog.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
	prog.setUniform("Light.Position", vec4(0.0f,0.0f,0.0f, 1.0f));
	prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
	prog.setUniform("Light.Intensity", 1.0f, 1.0f, 1.0f);
	prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
	prog.setUniform("Material.Shininess", 100.0f);
	/////////////////////////////////////////////
}


void SceneShadeWire::update( float t )
{
    angle += 0.001f;
    if( angle > TWOPI_F) angle -= TWOPI_F;
}

void SceneShadeWire::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(3.0f * cos(angle),0.0f,3.0f * sin(angle));
    view = glm::lookAt(cameraPos,
                       vec3(0.0f,0.0f,0.0f),
                       vec3(0.0f,1.0f,0.0f));

    model = mat4(1.0f);
    setMatrices();
    ogre->render();

    glFinish();
}

void SceneShadeWire::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ViewportMatrix", viewport);
}

void SceneShadeWire::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;

    float w2 = width / 2.0f;
    float h2 = height / 2.0f;
    viewport = mat4( vec4(w2,0.0f,0.0f,0.0f),
                     vec4(0.0f,h2,0.0f,0.0f),
                     vec4(0.0f,0.0f,1.0f,0.0f),
                     vec4(w2+0, h2+0, 0.0f, 1.0f));

}

void SceneShadeWire::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/shadewire.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader %s failed to compile!\n%s",
               "shadewire.vs", prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/shadewire.gs",GLSLShader::GEOMETRY) )
    {
        printf("Geometry shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/shadewire.fs",GLSLShader::FRAGMENT))
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
