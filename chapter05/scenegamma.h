#ifndef SCENEGAMMA_H
#define SCENEGAMMA_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "torus.h"
#include "objmesh.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneGamma : public Scene
{
private:
    GLSLProgram prog;

    Plane plane;
    Torus torus;
    std::unique_ptr<ObjMesh> ogre;

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
