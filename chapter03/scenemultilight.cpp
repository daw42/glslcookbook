#include "scenemultilight.h"

#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "glutils.h"

#include <iostream>
using std::endl;
using std::cerr;

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneMultiLight::SceneMultiLight()
{
}

void SceneMultiLight::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(10.0f, 10.0f, 100, 100);
    mesh = new VBOMesh("../media/pig_triangulated.obj",true);

    view = glm::lookAt(vec3(0.5f,0.75f,0.75f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    float x, z;
    for( int i = 0; i < 5; i++ ) {
		std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 5) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 5) * i);
        prog.setUniform(name.str().c_str(), view * vec4(x, 1.2f, z + 1.0f, 1.0f) );
    }

    prog.setUniform("lights[0].Intensity", vec3(0.0f,0.8f,0.8f) );
    prog.setUniform("lights[1].Intensity", vec3(0.0f,0.0f,0.8f) );
    prog.setUniform("lights[2].Intensity", vec3(0.8f,0.0f,0.0f) );
    prog.setUniform("lights[3].Intensity", vec3(0.0f,0.8f,0.0f) );
    prog.setUniform("lights[4].Intensity", vec3(0.8f,0.8f,0.8f) );
}

void SceneMultiLight::update( float t )
{

}

void SceneMultiLight::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f,1.0f,0.0f));
    setMatrices();
    mesh->render();

    prog.setUniform("Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model,vec3(0.0f,-0.45f,0.0f));
    setMatrices();
    plane->render();
}

void SceneMultiLight::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneMultiLight::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneMultiLight::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/multilight.vert",GLSLShader::VERTEX);
		prog.compileShader("shader/multilight.frag",GLSLShader::FRAGMENT);
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
