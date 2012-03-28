#ifndef SCENEAO_H
#define SCENEAO_H

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

class SceneAo : public Scene
{
private:
    GLSLProgram prog;

    VBOTeapot *teapot;
    VBOPlane *plane;
    VBOTorus *torus;
    VBOMesh *ogre;

    int width, height;

    mat4 model, view, projection;
    vec4 lightPos;
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
