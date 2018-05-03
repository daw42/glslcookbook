#ifndef SCENEQUADTESS_H
#define SCENEQUADTESS_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneQuadTess : public Scene
{
private:
    GLSLProgram prog;

    GLuint vaoHandle;
    glm::mat4 viewport;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneQuadTess();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEQUADTESS_H
