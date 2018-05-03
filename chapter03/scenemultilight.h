#ifndef SCENEMULTILIGHT_H
#define SCENEMULTILIGHT_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "objmesh.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneMultiLight : public Scene
{
private:
    GLSLProgram prog;

    Plane plane;
    std::unique_ptr<ObjMesh> mesh;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneMultiLight();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEMULTILIGHT_H
