#ifndef SCENEPARTICLESINSTANCED_H
#define SCENEPARTICLESINSTANCED_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"
#include "torus.h"

#include <glm/glm.hpp>

class SceneParticlesInstanced : public Scene
{
private:
    GLSLProgram prog;

    //GLuint particleArray;
    GLuint initVel, startTime;
    Torus torus;

    int nParticles;
    float angle;
    float time, deltaT;

    void setMatrices();
    void compileAndLinkShader();
    void initBuffers();
    float randFloat();

public:
    SceneParticlesInstanced();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPARTICLESINSTANCED_H
