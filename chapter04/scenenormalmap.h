#ifndef SCENENORMALMAP_H
#define SCENENORMALMAP_H

#include "scene.h"
#include "glslprogram.h"
#include "objmesh.h"

#include <glm/glm.hpp>

class SceneNormalMap : public Scene
{
private:
    GLSLProgram prog;

    std::unique_ptr<ObjMesh> ogre;

    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneNormalMap();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENENORMALMAP_H
