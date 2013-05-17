#ifndef SCENEGAMMA_H
#define SCENEGAMMA_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomesh.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneGamma : public Scene
{
private:
    GLSLProgram prog;

    int width, height;

    VBOPlane *plane;
    VBOTorus *torus;
    VBOMesh *ogre;
    VBOCube *cube;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();

public:
    SceneGamma();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEGAMMA_H
