#ifndef SCENEEDGE_H
#define SCENEEDGE_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "torus.h"
#include "teapot.h"
#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneEdge : public Scene
{
private:
    GLSLProgram prog, computeProg;

    GLuint fsQuad, pass1Index, pass2Index, fboHandle;

    Plane plane;
    Torus torus;
    Teapot teapot;

    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();

public:
    SceneEdge();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEEDGE_H
