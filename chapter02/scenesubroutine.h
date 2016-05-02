#ifndef SCENESUBROUTINE_H
#define SCENESUBROUTINE_H

#include "scene.h"
#include "glslprogram.h"
#include "vboteapot.h"
#include "vbotorus.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneSubroutine : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOTorus *torus;
    VBOTeapot *teapot;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneSubroutine();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENESUBROUTINE_H
