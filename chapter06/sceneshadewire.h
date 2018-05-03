#ifndef SCENESHADEWIRE_H
#define SCENESHADEWIRE_H

#include "scene.h"
#include "glslprogram.h"
#include "objmesh.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneShadeWire : public Scene
{
private:
    GLSLProgram prog;

    std::unique_ptr<ObjMesh> ogre;

    glm::mat4 viewport;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();

public:
    SceneShadeWire();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENESHADEWIRE_H
