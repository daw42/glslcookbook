#ifndef SCENEMSAA_H
#define SCENEMSAA_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomesh.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneMsaa : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    GLuint quad;

    VBOPlane *plane;

    mat4 model;
    mat4 view;
    mat4 projection;
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
