#ifndef SCENESKY_H
#define SCENESKY_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneSky : public Scene
{
private:
    GLSLProgram prog;

    GLuint quad;

    glm::vec3 lightPos;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void drawScene();

public:
    SceneSky();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENESKY_H
