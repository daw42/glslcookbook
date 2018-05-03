#ifndef SCENETESSTEAPOTDEPTH_H
#define SCENETESSTEAPOTDEPTH_H

#include "scene.h"
#include "glslprogram.h"
#include "teapotpatch.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneTessTeapotDepth : public Scene
{
private:
    GLSLProgram prog;

    GLuint vaoHandle;

    TeapotPatch teapot;

    glm::mat4 viewport;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTessTeapotDepth();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENETESSTEAPOTDEPTH_H
