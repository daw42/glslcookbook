#ifndef SCENESUBROUTINE_H
#define SCENESUBROUTINE_H

#include "scene.h"
#include "glslprogram.h"
#include "teapot.h"
#include "torus.h"

#include "cookbookogl.h"

class SceneSubroutine : public Scene
{
private:
    GLSLProgram prog;

    Teapot teapot;

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
