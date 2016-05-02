#ifndef SCENETEXTURE_H
#define SCENETEXTURE_H

// OpenGL headers
#include "cookbookogl.h"

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneTexture : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOCube *cube;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTexture();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPERFRAGMENT_H
