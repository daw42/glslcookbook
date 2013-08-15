#ifndef SCENEHDRBLOOM_H
#define SCENEHDRBLOOM_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbosphere.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneHdrBloom : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    GLuint fsQuad, pass1Index, pass2Index, pass3Index, pass4Index, pass5Index;
    GLuint hdrFbo, blurFbo;
    GLuint hdrTex, tex1, tex2;
    GLuint linearSampler, nearestSampler;

    VBOPlane *plane;
    VBOTeapot *teapot;
    VBOSphere *sphere;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle, tPrev, rotSpeed;
    int bloomBufWidth, bloomBufHeight;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();
    void pass3();
    void pass4();
    void pass5();
    float gauss(float, float);
    void drawScene();
    void computeLogAveLuminance();

public:
    SceneHdrBloom();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEHDRBLOOM_H
