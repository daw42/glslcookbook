#ifndef SCENESPOT_H
#define SCENESPOT_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "teapot.h"
#include "torus.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneSpot : public Scene
{
private:
    GLSLProgram prog;

    float tPrev;
    Plane plane;
    Teapot teapot;
    Torus torus;

    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneSpot();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENESPOT_H
