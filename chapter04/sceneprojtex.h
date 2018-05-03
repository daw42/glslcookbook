#ifndef SCENEPROJTEX_H
#define SCENEPROJTEX_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "teapot.h"

#include <glm/glm.hpp>

class SceneProjTex : public Scene
{
private:
    GLSLProgram prog;

    Teapot teapot;
    Plane plane;
    
    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneProjTex();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif
