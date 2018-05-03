#ifndef SCENETESSTEAPOT_H
#define SCENETESSTEAPOT_H

#include "scene.h"
#include "glslprogram.h"
#include "teapotpatch.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneTessTeapot : public Scene
{
private:
    GLSLProgram prog;

    GLuint vaoHandle;

    TeapotPatch teapot;
    glm::mat4 viewport;
    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTessTeapot();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENETESSTEAPOT_H
