#ifndef SCENEFOG_H
#define SCENEFOG_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "teapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneFog : public Scene
{
private:
    GLSLProgram prog;

    float tPrev;
    Plane plane;
    Teapot teapot;

    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneFog();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEFog_H
