#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "scene.h"

#include "cookbookogl.h"
#include "glslprogram.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneBasic_Uniform : public Scene
{
private:
    int width, height;
    GLuint vaoHandle;
    GLSLProgram prog;
    float angle;

    mat4 rotationMatrix;

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
