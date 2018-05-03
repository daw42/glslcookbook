#ifndef SCENEAO_H
#define SCENEAO_H

#include "scene.h"
#include "glslprogram.h"
#include "objmesh.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneAo : public Scene
{
private:
    GLSLProgram prog;

    std::unique_ptr<ObjMesh> ogre;
    glm::vec4 lightPos;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void drawScene();

public:
    SceneAo();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEAO_H
