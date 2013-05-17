#include "scenerefractcube.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
using std::cout;
using std::endl;

#include "bmpreader.h"

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneRefractCube::SceneRefractCube() : angle(0.0f), tPrev(0.0f), rotSpeed(PI/4.0) { }

void SceneRefractCube::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(14, mat4(1.0f));
    sky = new SkyBox();
    plane = new VBOPlane(1.0f,1.0f,1,1);
    float c = 3.5f;
    torus = new VBOTorus(0.7f * c, 0.3f * c, 50, 50);

    projection = mat4(1.0f);

    angle = (float)( TO_RADIANS(90.0) );
	
    loadCubeMap("../media/texture/cubemap_night/night");
}

void SceneRefractCube::loadCubeMap( const char * baseFileName )
{
     glActiveTexture(GL_TEXTURE0);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    GLuint targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for( int i = 0; i < 6; i++ ) {
		string texName = string(baseFileName) + "_" + suffixes[i] + ".bmp";
		GLuint w, h;
		GLubyte * data = BMPReader::load(texName.c_str(), w, h);
		glTexImage2D(targets[i], 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		delete [] data;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    prog.setUniform("CubeMapTex", 0);
}

void SceneRefractCube::update( float t )
{
	float deltaT = t - tPrev;
	if(tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;

    angle += rotSpeed * deltaT;
    if( angle > TWOPI_F) angle -= TWOPI_F;
}

void SceneRefractCube::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3( 7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    prog.setUniform("WorldCameraPosition", cameraPos);
    //view = glm::lookAt(vec3(0.0f,2.0f,0.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,0.0f,1.0f));

    prog.setUniform("DrawSkyBox", true);
    model = mat4(1.0f);
    setMatrices();
    sky->render();
    prog.setUniform("DrawSkyBox", false);

    prog.setUniform("Material.Eta", 0.94f);
    prog.setUniform("Material.ReflectionFactor", 0.1f);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,-1.0f,0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
}

void SceneRefractCube::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelMatrix", model);
    prog.setUniform("MVP", projection * mv);
}

void SceneRefractCube::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(50.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneRefractCube::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/cubemap_refract.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/cubemap_refract.fs",GLSLShader::FRAGMENT))
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
