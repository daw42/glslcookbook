#include "scenegamma.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"

#include <iostream>
using std::endl;
using std::cerr;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

SceneGamma::SceneGamma() : width(800), height(600), angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f)
{ }

void SceneGamma::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    float c = 2.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    c = 1.5f;
    torus = new VBOTorus(0.7f * c, 0.3f * c, 50,50);
    ogre = new VBOMesh("../media/bs_ears.obj");

	angle = glm::pi<float>() / 2.0f;

    prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f) );
    prog.setUniform("Gamma",2.2f);
}


void SceneGamma::update( float t )
{
	float deltaT = t - tPrev;
	if(tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;

    angle += rotSpeed * deltaT;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneGamma::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(vec3(3.0f * cos(angle),0.0f,3.0f * sin(angle)), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));

    prog.setUniform("Light.Position", view * vec4(10.0f,0.0f,0.0f,1.0f) );
    prog.setUniform("Material.Kd", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    setMatrices();
    ogre->render();

    glFinish();
}

void SceneGamma::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneGamma::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneGamma::compileAndLinkShader()
{
	try {
    	prog.compileShader("shader/gamma.vs");
    	prog.compileShader("shader/gamma.fs");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
