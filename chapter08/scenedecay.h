#ifndef SCENEDECAY_H
#define SCENEDECAY_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

#include "vboteapot.h"

class SceneDecay : public Scene
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
    SceneDecay();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEDECAY_H
