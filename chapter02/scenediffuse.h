#ifndef SCENEDIFFUSE_H
#define SCENEDIFFUSE_H

#include "scene.h"
#include "glslprogram.h"
#include "vbotorus.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneDiffuse : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDiffuse();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEDIFFUSE_H
