#ifndef SCENESILHOUETTE_H
#define SCENESILHOUETTE_H

#include "scene.h"
#include "glslprogram.h"
#include "objmesh.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneSilhouette : public Scene
{
private:
    GLSLProgram prog;

    std::unique_ptr<ObjMesh> ogre;
    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();

public:
    SceneSilhouette();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENESILHOUETTE_H
