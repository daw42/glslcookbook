#ifndef SCENEMANDELBROT_H
#define SCENEMANDELBROT_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"
#include "cube.h"

#include <glm/glm.hpp>

class SceneMandelbrot : public Scene
{
private:
    GLSLProgram renderProg, computeProg;

    GLuint dataBuf, fsQuad;
    Cube cube;

    glm::vec2 center;
    float cheight;

    float time, deltaT, speed;
    float angle, rotSpeed;

    void compileAndLinkShader();
    void initBuffers();
    void setWindow();

public:
    SceneMandelbrot();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEMANDELBROT_H
