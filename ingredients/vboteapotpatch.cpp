#include "vboteapotpatch.h"
#include "teapotdata.h"
#include "glutils.h"

#include "cookbookogl.h"

#include <cstdio>

#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec4;

VBOTeapotPatch::VBOTeapotPatch()
{
    int verts = 32 * 16;
    float * v = new float[ verts * 3 ];

    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    unsigned int handle;
    glGenBuffers(1, &handle);

    generatePatches( v );

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(float), v, GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);  // Vertex position

    delete [] v;

    glBindVertexArray(0);
}

void VBOTeapotPatch::generatePatches(float * v) {
    int idx = 0;

    // Build each patch
    // The rim
    buildPatchReflect(0, v, idx, true, true);
    // The body
    buildPatchReflect(1, v, idx, true, true);
    buildPatchReflect(2, v, idx, true, true);
    // The lid
    buildPatchReflect(3, v, idx, true, true);
    buildPatchReflect(4, v, idx, true, true);
    // The bottom
    buildPatchReflect(5, v, idx, true, true);
    // The handle
    buildPatchReflect(6, v, idx, false, true);
    buildPatchReflect(7, v, idx, false, true);
    // The spout
    buildPatchReflect(8, v, idx, false, true);
    buildPatchReflect(9, v, idx, false, true);
}

void VBOTeapotPatch::buildPatchReflect(int patchNum,
           float *v, int &index, bool reflectX, bool reflectY)
{
    vec3 patch[4][4];
    vec3 patchRevV[4][4];
    getPatch(patchNum, patch, false);
    getPatch(patchNum, patchRevV, true);

    // Patch without modification
    buildPatch(patchRevV, v, index, mat3(1.0f));

    // Patch reflected in x
    if( reflectX ) {
        buildPatch(patch, v,
                   index, mat3(vec3(-1.0f, 0.0f, 0.0f),
                               vec3(0.0f, 1.0f, 0.0f),
                               vec3(0.0f, 0.0f, 1.0f) ) );
    }

    // Patch reflected in y
    if( reflectY ) {
        buildPatch(patch, v,
                   index, mat3(vec3(1.0f, 0.0f, 0.0f),
                               vec3(0.0f, -1.0f, 0.0f),
                               vec3(0.0f, 0.0f, 1.0f) ) );
    }

    // Patch reflected in x and y
    if( reflectX && reflectY ) {
        buildPatch(patchRevV, v,
                   index, mat3(vec3(-1.0f, 0.0f, 0.0f),
                               vec3(0.0f, -1.0f, 0.0f),
                               vec3(0.0f, 0.0f, 1.0f) ) );
    }
}

void VBOTeapotPatch::buildPatch(vec3 patch[][4],
                           float *v, int &index, mat3 reflect )
{
    for( int i = 0; i < 4; i++ )
    {
        for( int j = 0 ; j < 4; j++)
        {
            vec3 pt = reflect * patch[i][j];

            v[index] = pt.x;
            v[index+1] = pt.y;
            v[index+2] = pt.z;

            index += 3;
        }
    }
}

void VBOTeapotPatch::getPatch( int patchNum, vec3 patch[][4], bool reverseV )
{
    for( int u = 0; u < 4; u++) {          // Loop in u direction
        for( int v = 0; v < 4; v++ ) {     // Loop in v direction
            if( reverseV ) {
                patch[u][v] = vec3(
                        Teapot::cpdata[Teapot::patchdata[patchNum][u*4+(3-v)]][0],
                        Teapot::cpdata[Teapot::patchdata[patchNum][u*4+(3-v)]][1],
                        Teapot::cpdata[Teapot::patchdata[patchNum][u*4+(3-v)]][2]
                        );
            } else {
                patch[u][v] = vec3(
                        Teapot::cpdata[Teapot::patchdata[patchNum][u*4+v]][0],
                        Teapot::cpdata[Teapot::patchdata[patchNum][u*4+v]][1],
                        Teapot::cpdata[Teapot::patchdata[patchNum][u*4+v]][2]
                        );
            }
        }
    }
}

void VBOTeapotPatch::render() const {
    glPatchParameteri(GL_PATCH_VERTICES, 16);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_PATCHES, 0, 512);
}
