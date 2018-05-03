#ifndef SCENENOISE_H
#define SCENENOISE_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneNoise : public Scene
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
    SceneNoise();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENENOISE_H
