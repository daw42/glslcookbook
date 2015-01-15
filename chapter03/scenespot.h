#ifndef SCENESPOT_H
#define SCENESPOT_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vboteapot.h"
#include "vbotorus.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneSpot : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    float tPrev;
    VBOPlane *plane;
    VBOTeapot *teapot;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;
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
