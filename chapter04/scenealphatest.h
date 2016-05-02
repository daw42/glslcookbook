#ifndef SCENEALPHATEST_H
#define SCENEALPHATEST_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboteapot.h"

// OpenGL headers
#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneAlphaTest : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOTeapot *teapot;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneAlphaTest();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEALPHATEST_H
