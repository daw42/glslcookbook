#ifndef SCENETONEMAP_H
#define SCENETONEMAP_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "teapot.h"
#include "sphere.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneToneMap : public Scene
{
private:
    GLSLProgram prog;

    GLuint hdrFBO;
    GLuint quad, pass1Index, pass2Index;
    GLuint hdrTex, avgTex;

    Plane plane;
    Teapot teapot;
    Sphere sphere;

    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();
    void computeLogAveLuminance();
    void drawScene();

public:
    SceneToneMap();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif 
