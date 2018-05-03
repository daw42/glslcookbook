#ifndef SCENEDEFERRED_H
#define SCENEDEFERRED_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "torus.h"
#include "teapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneDeferred : public Scene
{
private:
    GLSLProgram prog;

    GLuint deferredFBO;
    GLuint quad, pass1Index, pass2Index;

    Plane plane;
    Torus torus;
    Teapot teapot;

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
