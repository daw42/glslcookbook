#ifndef SCENEDIRECTIONAL_H
#define SCENEDIRECTIONAL_H

#include "scene.h"
#include "glslprogram.h"
#include "torus.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneDirectional : public Scene
{
private:
    GLSLProgram prog;

    Torus torus;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDirectional();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEDIRECTIONAL_H
