#ifndef SCENEWOOD_H
#define SCENEWOOD_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneWood : public Scene
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
    SceneWood();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEWOOD_H
