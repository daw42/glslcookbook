#ifndef SCENEDIRECTIONAL_H
#define SCENEDIRECTIONAL_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbotorus.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneDirectional : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOPlane *plane;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDirectional();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEDIRECTIONAL_H
