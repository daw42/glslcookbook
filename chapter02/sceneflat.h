#ifndef SCENEFLAT_H
#define SCENEFLAT_H

#include "scene.h"
#include "glslprogram.h"
#include "objmesh.h"

#include "cookbookogl.h"

class SceneFlat : public Scene
{
private:
    GLSLProgram prog;

    std::unique_ptr<ObjMesh> ogre;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneFlat();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEFLAT_H
