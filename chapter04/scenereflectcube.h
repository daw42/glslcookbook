#ifndef SCENEREFLECTCUBE_H
#define SCENEREFLECTCUBE_H

#include "scene.h"
#include "glslprogram.h"
#include "teapot.h"
#include "skybox.h"

#include <glm/glm.hpp>

class SceneReflectCube : public Scene
{
private:
    GLSLProgram prog;

    Teapot teapot;
    SkyBox sky;

    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void loadCubeMap( const char * baseFileName );

public:
    SceneReflectCube();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEREFLECTCUBE_H
