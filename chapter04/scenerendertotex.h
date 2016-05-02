#ifndef SCENERENDERTOTEX_H
#define SCENERENDERTOTEX_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboteapot.h"

// OpenGL headers
#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneRenderToTex : public Scene
{
private:
    GLSLProgram prog;

    GLuint fboHandle;
    int width, height;
    VBOCube *cube;
    VBOTeapot *teapot;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;
    float tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void renderToTexture();
    void renderScene();

public:
    SceneRenderToTex();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENERENDERTOTEX_H
