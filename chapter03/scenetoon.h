#ifndef SCENETOON_H
#define SCENETOON_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vboteapot.h"
#include "vbotorus.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneToon : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    float tPrev;
    VBOPlane *plane;
    VBOTeapot *teapot;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneToon();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENETOON_H
