#include "scenesubroutine.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneSubroutine::SceneSubroutine()
{
}

void SceneSubroutine::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(13, mat4(1.0f));
    torus = new VBOTorus(0.7f, 0.3f, 30, 30);

    view = glm::lookAt(vec3(0.0f,0.0f,10.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Shininess", 100.0f);

}

void SceneSubroutine::update( float t ) { }

void SceneSubroutine::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4 lightPos = vec4(0.0f,0.0f,0.0f,1.0f);
    prog.setUniform("Light.Position", lightPos );

    GLuint programHandle = prog.getHandle();

    GLuint adsIndex = glGetSubroutineIndex( programHandle,
                                            GL_VERTEX_SHADER,
                                            "phongModel" );
    GLuint diffuseIndex = glGetSubroutineIndex(programHandle,
                                               GL_VERTEX_SHADER,
                                               "diffuseOnly");

    glUniformSubroutinesuiv( GL_VERTEX_SHADER, 1, &adsIndex);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-3.0,-1.5,0.0));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    glUniformSubroutinesuiv( GL_VERTEX_SHADER, 1, &diffuseIndex);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(3.0f, -1.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
}

void SceneSubroutine::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneSubroutine::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(50.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneSubroutine::compileAndLinkShader()
{
    try {
    	prog.compileShader("shader/subroutine.vert");
    	prog.compileShader("shader/subroutine.frag");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
  		exit( EXIT_FAILURE );
    }
}
