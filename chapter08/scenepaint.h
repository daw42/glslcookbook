#ifndef SCENEPAINT_H
#define SCENEPAINT_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

#include "vboteapot.h"

class ScenePaint : public Scene
{
private:
    GLSLProgram prog;

    VBOTeapot *teapot;

    GLuint quad;
    int width, height;

    mat4 model, view, projection;
    vec4 lightPos;
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
