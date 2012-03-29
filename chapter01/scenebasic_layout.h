#ifndef SCENEBASIC_LAYOUT_H
#define SCENEBASIC_LAYOUT_H

#include "scene.h"

#include "cookbookogl.h"

class SceneBasic_Layout : public Scene
{
private:
    int width, height;
    GLuint vboHandles[2];
    GLuint vaoHandle;

    void linkMe(GLint vertShader, GLint fragShader);
    void printActiveAttribs(GLuint handle);

public:
    SceneBasic_Layout();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_LAYOUT_H
