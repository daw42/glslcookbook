#ifndef SCENEJITTER_H
#define SCENEJITTER_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomesh.h"
#include "frustum.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

class SceneJitter : public Scene
{
private:
    GLSLProgram prog;
    GLuint shadowFBO, pass1Index, pass2Index;

    VBOTeapot *teapot;
    VBOPlane *plane;
    VBOTorus *torus;
    VBOMesh *mesh;

    Frustum *lightFrustum;

    int width, height;
    int samplesU, samplesV;
    int jitterMapSize;
    float radius;
    int shadowMapWidth, shadowMapHeight;

    mat4 model, view, projection;
    mat4 lightView, lightProjection, lightPV;
    mat4 shadowScale;
    vec3 lightPos;
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
