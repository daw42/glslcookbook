#ifndef SCENEBASIC_UNIFORMBLOCK_H
#define SCENEBASIC_UNIFORMBLOCK_H

#include "scene.h"

#include "cookbookogl.h"
#include "glslprogram.h"

#include <glm/glm.hpp>

class SceneBasic_UniformBlock : public Scene
{
private:
    GLuint vaoHandle;
    GLSLProgram prog;
    float angle;

    void compile();
    void initUniformBlockBuffer();

public:
    SceneBasic_UniformBlock();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORMBLOCK_H
