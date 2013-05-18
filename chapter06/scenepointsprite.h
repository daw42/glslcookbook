#ifndef SCENEPOINTSPRITE_H
#define SCENEPOINTSPRITE_H

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

class ScenePointSprite : public Scene
{
private:
    GLSLProgram prog;

    GLuint sprites;

    VBOPlane *plane;
    VBOTorus *torus;
    VBOMesh *ogre;
    VBOCube *cube;

    int numSprites;
    float *locations;

    mat4 model;
    mat4 view;
    mat4 projection;

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
