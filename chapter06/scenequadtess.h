#ifndef SCENEQUADTESS_H
#define SCENEQUADTESS_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomeshadj.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneQuadTess : public Scene
{
private:
    GLSLProgram prog;

    GLuint vaoHandle;
    int width, height;

    mat4 model;
    mat4 view, viewport;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneQuadTess();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEQUADTESS_H
