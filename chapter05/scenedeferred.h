#ifndef SCENEDEFERRED_H
#define SCENEDEFERRED_H

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

class SceneDeferred : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    GLuint deferredFBO;
    GLuint quad, pass1Index, pass2Index;

    VBOPlane *plane;
    VBOTorus *torus;
    VBOMesh *ogre;
    VBOCube *cube;
    VBOTeapot *teapot;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void createGBufTex(GLenum, GLenum, GLuint &);
    void pass1();
    void pass2();

public:
    SceneDeferred();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEDEFERRED_H
