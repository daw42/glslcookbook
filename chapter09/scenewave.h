#ifndef SCENEWAVE_H
#define SCENEWAVE_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneWave : public Scene
{
private:
    GLSLProgram prog;

    Plane plane;

    float angle;
    float time;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneWave();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEWAVE_H
