#ifndef SCENEREFLECTCUBE_H
#define SCENEREFLECTCUBE_H

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

class SceneReflectCube : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOTeapot *teapot;
    VBOTorus *torus;
    VBOPlane *plane;
    SkyBox *sky;

    mat4 model;
    mat4 view;
    mat4 projection;
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
