#ifndef SCENEPARTICLESINSTANCED_H
#define SCENEPARTICLESINSTANCED_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneParticlesInstanced : public Scene
{
private:
    GLSLProgram prog;

    int width, height;

    GLuint particleArray;
    GLuint initVel, startTime;

    int nParticles;

    VBOPlane *plane;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;
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
