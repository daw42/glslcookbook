#ifndef SCENESHADOWMAP_H
#define SCENESHADOWMAP_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "torus.h"
#include "teapot.h"
#include "frustum.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneShadowMap : public Scene
{
private:
    GLSLProgram prog, solidProg;
    GLuint shadowFBO, pass1Index, pass2Index;

    Teapot teapot;
    Plane plane;
    Torus torus;

    int shadowMapWidth, shadowMapHeight;
    float tPrev;

    glm::mat4 lightPV, shadowBias;
    float angle;

    Frustum lightFrustum;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void drawScene();
    void spitOutDepthBuffer();

public:
    SceneShadowMap();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENESHADOWMAP_H
