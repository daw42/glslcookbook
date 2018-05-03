#ifndef SCENEADS_H
#define SCENEADS_H

#include "scene.h"
#include "glslprogram.h"
#include "torus.h"
#include "cookbookogl.h"

class SceneADS : public Scene
{
private:
    GLSLProgram prog;

    Torus torus;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneADS();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEADS_H
