#ifndef VBOMESHADJ_H
#define VBOMESHADJ_H

#include "drawable.h"

#include <vector>
using std::vector;
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec2;
using glm::vec4;

#include <string>
using std::string;

#include "cookbookogl.h"

class VBOMeshAdj : public Drawable
{
private:
    GLuint faces;
    GLuint vaoHandle;

    void trimString( string & str );
    void determineAdjacency(
            vector<GLuint> & el
            );
    void storeVBO( const vector<vec3> & points,
                            const vector<vec3> & normals,
                            const vector<vec2> &texCoords,
                            const vector<vec4> &tangents,
                            const vector<GLuint> &elements );
    void generateAveragedNormals(
            const vector<vec3> & points,
            vector<vec3> & normals,
            const vector<GLuint> & faces );
    void generateTangents(
            const vector<vec3> & points,
            const vector<vec3> & normals,
            const vector<GLuint> & faces,
            const vector<vec2> & texCoords,
            vector<vec4> & tangents);
    void center(vector<vec3> &);

public:
    VBOMeshAdj( const char * fileName, bool reCenterMesh = false );

    void render() const;

    void loadOBJ( const char * fileName, bool );
};

#endif // VBOMESH_H
