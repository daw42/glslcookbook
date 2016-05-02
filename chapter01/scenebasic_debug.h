#ifndef SCENEBASIC_DEBUG_H
#define SCENEBASIC_DEBUG_H

#include "scene.h"

#include "cookbookogl.h"
#include "glslprogram.h"

class SceneBasic_Debug : public Scene
{
private:
    int width, height;
    GLuint vaoHandle;
    GLSLProgram prog;

    void compileShaderProgram();

public:
    SceneBasic_Debug();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_DEBUG_H
