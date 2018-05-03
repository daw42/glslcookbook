#ifndef SCENENIGHTVISION_H
#define SCENENIGHTVISION_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "torus.h"
#include "teapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneNightVision : public Scene
{
private:
    GLSLProgram prog;

    GLuint fsQuad, pass1Index, pass2Index;
    GLuint renderFBO;
    GLuint renderTex;
    GLuint noiseTex;

    Plane plane;
    Torus torus;
    Teapot teapot;

    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();

public:
    SceneNightVision();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENENIGHTVISION_H
