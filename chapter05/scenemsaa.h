#ifndef SCENEMSAA_H
#define SCENEMSAA_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneMsaa : public Scene
{
private:
    GLSLProgram prog;

    GLuint quad;

    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();

public:
    SceneMsaa();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEMSAA_H
