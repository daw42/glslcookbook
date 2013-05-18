#ifndef SCENESILHOUETTE_H
#define SCENESILHOUETTE_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomeshadj.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneSilhouette : public Scene
{
private:
    GLSLProgram prog;

    VBOMeshAdj *ogre;

    mat4 model;
    mat4 view, viewport;
    mat4 projection;
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
