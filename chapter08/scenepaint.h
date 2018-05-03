#ifndef SCENEPAINT_H
#define SCENEPAINT_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"
#include "teapot.h"

#include <glm/glm.hpp>

class ScenePaint : public Scene
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
    ScenePaint();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPAINT_H
