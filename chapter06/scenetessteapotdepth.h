#ifndef SCENETESSTEAPOTDEPTH_H
#define SCENETESSTEAPOTDEPTH_H

#include "scene.h"
#include "glslprogram.h"
#include "vboteapotpatch.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneTessTeapotDepth : public Scene
{
private:
    GLSLProgram prog;

    GLuint vaoHandle;

    VBOTeapotPatch *teapot;

    mat4 model;
    mat4 view, viewport;
    mat4 projection;

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
