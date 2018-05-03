#include "scenediffuse.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;


SceneDiffuse::SceneDiffuse() : torus(0.7f, 0.3f, 30, 30)
{ }

void SceneDiffuse::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f,0.0f,0.0f));
    model = glm::rotate(model, glm::radians(35.0f), vec3(0.0f,1.0f,0.0f));
    view = glm::lookAt(vec3(0.0f,0.0f,2.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("LightPosition", view * glm::vec4(5.0f,5.0f,2.0f,1.0f) );
}

void SceneDiffuse::update( float t )
{

}

void SceneDiffuse::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setMatrices();
    torus.render();
}

void SceneDiffuse::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneDiffuse::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneDiffuse::compileAndLinkShader()
{
	try {
    	prog.compileShader("shader/diffuse.vert");
    	prog.compileShader("shader/diffuse.frag");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
   		cerr << e.what() << endl;
   		exit( EXIT_FAILURE );
    }
}
