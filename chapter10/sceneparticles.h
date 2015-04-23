#ifndef SCENEPARTICLES_H
#define SCENEPARTICLES_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneParticles : public Scene
{
private:
    GLSLProgram renderProg, computeProg;

    int width, height;
    glm::ivec3 nParticles;
    GLuint totalParticles;

    float time, deltaT, speed, angle;
    GLuint particlesVao;
    GLuint bhVao, bhBuf;  // black hole VAO and buffer
    glm::vec4 bh1, bh2;

    glm::mat4 view, model, projection;

    void compileAndLinkShader();
    void initBuffers();
    void setMatrices();

public:
    SceneParticles();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SceneParticles_H
