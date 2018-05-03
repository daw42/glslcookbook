#ifndef SCENEJITTER_H
#define SCENEJITTER_H

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "objmesh.h"
#include "frustum.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

class SceneJitter : public Scene
{
private:
    GLSLProgram prog;
    GLuint shadowFBO, pass1Index, pass2Index;

    Plane plane;
    std::unique_ptr<ObjMesh> mesh;
    Frustum lightFrustum;

    int samplesU, samplesV;
    int jitterMapSize;
    float radius;
    int shadowMapWidth, shadowMapHeight;

    glm::mat4 lightPV;
    glm::mat4 shadowScale;
    glm::vec3 lightPos;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void drawScene();
    float jitter();
    void buildJitterTex();
    void drawBuildingScene();

public:
    SceneJitter();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEJITTER_H
