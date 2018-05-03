#ifndef SCENEPERFRAGMENT_H
#define SCENEPERFRAGMENT_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "teapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class ScenePerFragment : public Scene
{
private:
    GLSLProgram prog;

    float tPrev;
    Plane plane;
    Teapot teapot;

    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    ScenePerFragment();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPERFRAGMENT_H
