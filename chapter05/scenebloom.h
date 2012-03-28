#ifndef SCENEBLOOM_H
#define SCENEBLOOM_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneBloom : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    GLuint fsQuad, pass1Index, pass2Index, pass3Index, pass4Index;
    GLuint renderFBO, fbo1, fbo2;
    GLuint renderTex, tex1, tex2;

    VBOPlane *plane;
    VBOTorus *torus;
    VBOTeapot *teapot;
    VBOCube *cube;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();
    void pass3();
    void pass4();
    float gauss(float, float);

public:
    SceneBloom();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBLOOM_H
