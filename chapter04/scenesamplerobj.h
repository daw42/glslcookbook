#ifndef SCENESAMPLEROBJ_H
#define SCENESAMPLEROBJ_H

// OpenGL headers
#include "cookbookogl.h"

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneSamplerObj : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOPlane *plane;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;
    GLuint linearSampler, nearestSampler;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneSamplerObj();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif
