#ifndef SCENEDIFFUSE_H
#define SCENEDIFFUSE_H

#include "scene.h"
#include "glslprogram.h"
#include "torus.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneDiffuse : public Scene
{
private:
    GLSLProgram prog;

    Torus torus;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDiffuse();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEDIFFUSE_H
