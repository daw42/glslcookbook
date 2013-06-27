#ifndef SCENEBASIC_LAYOUT_H
#define SCENEBASIC_LAYOUT_H

#include "scene.h"

#include "cookbookogl.h"
#include "glslprogram.h"

class SceneBasic_Attrib : public Scene
{
private:
    int width, height;
    GLuint vboHandles[2];
    GLuint vaoHandle;
    GLSLProgram prog;

    void compileShaderProgram();

public:
    SceneBasic_Attrib();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_LAYOUT_H
