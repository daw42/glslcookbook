#include "sceneao.h"

#include <cstdio>
#include <iostream>
using std::cerr;
using std::endl;
#include "bmpreader.h"

#include "glutils.h"

#include <cstdlib>

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneAo::SceneAo() {}

void SceneAo::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

	angle = glm::pi<float>() / 2.0f;

    ogre = new VBOMesh("../media/bs_ears.obj", false, true);

    lightPos = glm::vec4(0.0f,0.0f,0.0f,1.0f);  // Camera coords

    prog.setUniform("Light.Position", lightPos);
    prog.setUniform("Light.Intensity", vec3(1.0f));

    float c = 2.25f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);

    glActiveTexture(GL_TEXTURE0);
    const char * texName = "../media/texture/ao_ears.bmp";
    BMPReader::loadTex(texName);
    prog.setUniform("AOTex", 0);

    glActiveTexture(GL_TEXTURE1);
    const char * diffTexName = "../media/texture/diffuse.bmp";
    BMPReader::loadTex(diffTexName);
    prog.setUniform("DiffTex", 1);
}

void SceneAo::update( float t )
{
    angle += 0.003f;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneAo::render()
{
    view = glm::lookAt(vec3(3.0f * cos(angle),0.0f,3.0f * sin(angle)), vec3(0.0f), vec3(0.0f,1.0f,0.0f));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glFinish();
}

void SceneAo::drawScene()
{
    model = mat4(1.0f);
    setMatrices();
    ogre->render();
}

void SceneAo::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneAo::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneAo::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/ao.vs");
		prog.compileShader("shader/ao.fs");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
