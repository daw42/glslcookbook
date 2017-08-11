#include "sceneflat.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

SceneFlat::SceneFlat() : angle(0.0f) { }

void SceneFlat::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    ogre = new VBOMesh("../media/bs_ears.obj");

    view = glm::lookAt(vec3(0.0f,0.35f,0.85f), vec3(0.0f,-0.25f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Shininess", 100.0f);
}

void SceneFlat::update( float t )
{}

void SceneFlat::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    vec4 worldLight = vec4(2.0f,4.0f,1.0f,1.0f);
    model = glm::rotate(mat4(), glm::radians(angle), vec3(0.0f,1.0f,0.0f));
    prog.setUniform("Light.Position", view * model * worldLight );

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(25.0f), vec3(0.0f,1.0f,0.0f));

    setMatrices();
    ogre->render();
}

void SceneFlat::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneFlat::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneFlat::compileAndLinkShader()
{
	try {
    	prog.compileShader("shader/flat.vert");
    	prog.compileShader("shader/flat.frag");
    	prog.link();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
