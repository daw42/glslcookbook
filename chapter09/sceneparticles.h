#ifndef SCENEPARTICLES_H
#define SCENEPARTICLES_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneParticles : public Scene
{
private:
    GLSLProgram prog;

    GLuint initVel, startTime, particles;
    GLuint nParticles;

    float angle;
    float time;

    void setMatrices();
    void compileAndLinkShader();
    void initBuffers();
    float randFloat();

public:
    SceneParticles();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPARTICLES_H
