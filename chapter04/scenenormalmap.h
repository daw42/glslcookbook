#ifndef SCENENORMALMAP_H
#define SCENENORMALMAP_H

#include "scene.h"
#include "glslprogram.h"
#include "vbomesh.h"

// OpenGL headers
#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneNormalMap : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOMesh *ogre;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneNormalMap();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENENORMALMAP_H
