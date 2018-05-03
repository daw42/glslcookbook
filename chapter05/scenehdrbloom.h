#ifndef SCENEHDRBLOOM_H
#define SCENEHDRBLOOM_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "teapot.h"
#include "sphere.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneHdrBloom : public Scene
{
private:
    GLSLProgram prog;

    GLuint fsQuad, pass1Index, pass2Index, pass3Index, pass4Index, pass5Index;
    GLuint hdrFbo, blurFbo;
    GLuint hdrTex, tex1, tex2;
    GLuint linearSampler, nearestSampler;

    Plane plane;
    Teapot teapot;
    Sphere sphere;

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
