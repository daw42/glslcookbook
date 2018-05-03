#ifndef SCENEPOINTSPRITE_H
#define SCENEPOINTSPRITE_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"

class ScenePointSprite : public Scene
{
private:
    GLSLProgram prog;

    GLuint sprites;
    int numSprites;
    float *locations;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();

public:
    ScenePointSprite();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPOINTSPRITE_H
