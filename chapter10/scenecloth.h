#ifndef SCENECLOTH_H
#define SCENECLOTH_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"
#include "vboplane.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneCloth : public Scene
{
private:
    GLSLProgram renderProg, computeProg, computeProgNorm;

    int width, height;
    GLuint clothVao;
    GLuint numElements;

    glm::ivec2 nParticles;  // Number of particles in each dimension
    glm::vec2 clothSize;    // Size of cloth in x and y

    float time, deltaT, speed;
    GLuint readBuf;
    GLuint posBufs[2], velBufs[2];
    GLuint normBuf, elBuf, tcBuf;

    VBOPlane *groundPlane;

    glm::mat4 view, model, projection;

    void compileAndLinkShader();
    void initBuffers();
    void setMatrices();

public:
    SceneCloth();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENECLOTH_H
