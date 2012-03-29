#ifndef SCENETWOSIDE_H
#define SCENETWOSIDE_H

#include "scene.h"
#include "glslprogram.h"
#include "vboteapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneTwoSide : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOTeapot *teapot;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTwoSide();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENETWOSIDE_H
