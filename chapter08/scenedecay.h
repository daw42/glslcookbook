#ifndef SCENEDECAY_H
#define SCENEDECAY_H

#include "scene.h"
#include "glslprogram.h"
#include "teapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneDecay : public Scene
{
private:
    GLSLProgram prog;

    Teapot teapot;
    GLuint quad;

    glm::vec4 lightPos;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void drawScene();

public:
    SceneDecay();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEDECAY_H
