#ifndef SCENEFLAT_H
#define SCENEFLAT_H

#include "scene.h"
#include "glslprogram.h"
#include "vbomesh.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneFlat : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOMesh *ogre;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneFlat();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEFLAT_H
