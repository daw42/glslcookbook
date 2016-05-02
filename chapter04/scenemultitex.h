#ifndef SCENEMULTITEX_H
#define SCENEMULTITEX_H

#include "scene.h"
#include "glslprogram.h"
#include "vbocube.h"

// OpenGL headers
#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneMultiTex : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOCube *cube;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneMultiTex();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEMULTITEX_H
