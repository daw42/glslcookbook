#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "scene.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneBasic_Uniform : public Scene
{
private:
    int width, height;
    GLuint vboHandles[2];
    GLuint vaoHandle;
    GLuint programHandle;
    float angle;

    mat4 rotationMatrix;

    void linkMe(GLint vertShader, GLint fragShader);
    void printActiveUniforms(GLuint);

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
