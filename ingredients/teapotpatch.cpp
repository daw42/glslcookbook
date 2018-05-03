#include "teapotpatch.h"
#include "teapotdata.h"
#include "glutils.h"

#include "cookbookogl.h"

#include <cstdio>

#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec4;

TeapotPatch::TeapotPatch()
{
    std::vector<GLfloat> pts( 32 * 16 * 3 );
    generatePatches( pts );
    initBuffers(pts);
}

void TeapotPatch::initBuffers(std::vector<GLfloat> & pts) {
    GLuint buf = 0;
    glGenBuffers(1, &buf);
    buffers.push_back(buf);

    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(float), pts.data(), GL_STATIC_DRAW);

    glGenVertexArrays( 1, &vao );
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void TeapotPatch::generatePatches(std::vector<GLfloat> & pts) {
    int idx = 0;

    // Build each patch
    // The rim
    buildPatchReflect(0, pts, idx, true, true);
    // The body
    buildPatchReflect(1, pts, idx, true, true);
    buildPatchReflect(2, pts, idx, true, true);
    // The lid
    buildPatchReflect(3, pts, idx, true, true);
    buildPatchReflect(4, pts, idx, true, true);
    // The bottom
    buildPatchReflect(5, pts, idx, true, true);
    // The handle
    buildPatchReflect(6, pts, idx, false, true);
    buildPatchReflect(7, pts, idx, false, true);
    // The spout
    buildPatchReflect(8, pts, idx, false, true);
    buildPatchReflect(9, pts, idx, false, true);
}

void TeapotPatch::buildPatchReflect(int patchNum,
           std::vector<GLfloat> & p, int &index, bool reflectX, bool reflectY)
{
    glm::vec3 patch[4][4];
    glm::vec3 patchRevV[4][4];
    getPatch(patchNum, patch, false);
    getPatch(patchNum, patchRevV, true);

    // Patch without modification
    buildPatch(patchRevV, p, index, glm::mat3(1.0f));

    // Patch reflected in x
    if( reflectX ) {
        buildPatch(patch, p,
                   index, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f) ) );
    }

    // Patch reflected in y
    if( reflectY ) {
        buildPatch(patch, p,
                   index, glm::mat3(glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, -1.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f) ) );
    }

    // Patch reflected in x and y
    if( reflectX && reflectY ) {
        buildPatch(patchRevV, p,
                   index, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, -1.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f) ) );
    }
}

void TeapotPatch::buildPatch(glm::vec3 patch[][4],
                           std::vector<GLfloat> & pts, int &index, glm::mat3 reflect )
{
    for( int i = 0; i < 4; i++ )
    {
        for( int j = 0 ; j < 4; j++)
        {
            glm::vec3 pt = reflect * patch[i][j];

            pts[index] = pt.x;
            pts[index+1] = pt.y;
            pts[index+2] = pt.z;

            index += 3;
        }
    }
}

void TeapotPatch::getPatch( int patchNum, glm::vec3 patch[][4], bool reverseV )
{
    for( int u = 0; u < 4; u++) {          // Loop in u direction
        for( int v = 0; v < 4; v++ ) {     // Loop in v direction
            if( reverseV ) {
                patch[u][v] = glm::vec3(
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][0],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][1],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][2]
                        );
            } else {
                patch[u][v] = glm::vec3(
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][0],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][1],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][2]
                        );
            }
        }
    }
}

void TeapotPatch::render() const {
    glPatchParameteri(GL_PATCH_VERTICES, 16);

    glBindVertexArray(vao);
    glDrawArrays(GL_PATCHES, 0, 512);
}
