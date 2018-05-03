#ifndef SCENEOIT_H
#define SCENEOIT_H

#include "scene.h"
#include "glslprogram.h"
#include "cube.h"
#include "sphere.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

enum BufferNames {
  COUNTER_BUFFER = 0,
  LINKED_LIST_BUFFER
};

struct ListNode {
  glm::vec4 color;
  GLfloat depth;
  GLuint next;
};

class SceneOit : public Scene
{
private:
    GLSLProgram prog;

    GLuint buffers[2], fsQuad, headPtrTex;
    GLuint pass1Index, pass2Index;
    GLuint clearBuf;

    Cube cube;
    Sphere sphere;

    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void drawScene();
    void initShaderStorage();
    void pass1();
    void pass2();
    void clearBuffers();

public:
    SceneOit();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);

};

#endif // SCENEOIT_H
