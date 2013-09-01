#ifndef SCENESHADOWVOLUME_H
#define SCENESHADOWVOLUME_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbomeshadj.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

class SceneShadowVolume : public Scene
{
private:
    GLSLProgram volumeProg, renderProg, compProg;
    GLuint colorDepthFBO, fsQuad;
    GLuint spotTex, brickTex;

    VBOPlane *plane;
    VBOMeshAdj *spot;

    int width, height;

    vec4 lightPos;
    mat4 model, view, projection;
    float angle, tPrev, rotSpeed;

    void setMatrices(GLSLProgram &);
    void compileAndLinkShader();
    void setupFBO();
    void drawScene(GLSLProgram &, bool);
    void pass1();
    void pass2();
    void pass3();
    void updateLight();

public:
    SceneShadowVolume();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENESHADOWVOLUME_H
