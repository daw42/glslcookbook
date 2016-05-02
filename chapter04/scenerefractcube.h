#ifndef SCENEREFRACTCUBE_H
#define SCENEREFRACTCUBE_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbomesh.h"
#include "vboteapot.h"
#include "vbotorus.h"
#include "skybox.h"

// OpenGL headers
#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneRefractCube : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOTeapot *teapot;
    SkyBox *sky;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void loadCubeMap( const char * baseFileName );

public:
    SceneRefractCube();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEREFRACTCUBE_H
