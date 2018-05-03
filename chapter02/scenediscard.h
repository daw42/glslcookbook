#ifndef SCENEDISCARD_H
#define SCENEDISCARD_H

#include "scene.h"
#include "glslprogram.h"
#include "teapot.h"

#include "cookbookogl.h"

class SceneDiscard : public Scene
{
private:
    GLSLProgram prog;

    Teapot teapot;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDiscard();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEDISCARD_H
