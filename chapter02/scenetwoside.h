#ifndef SCENETWOSIDE_H
#define SCENETWOSIDE_H

#include "scene.h"
#include "glslprogram.h"
#include "teapot.h"

#include "cookbookogl.h"

class SceneTwoSide : public Scene
{
private:
    GLSLProgram prog;

    Teapot teapot;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTwoSide();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENETWOSIDE_H
