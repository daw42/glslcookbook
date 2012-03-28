#ifndef VBOMESH_H
#define VBOMESH_H

#include "drawable.h"

#include <vector>
using std::vector;
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec2;
using glm::vec4;

#include <string>
using std::string;

class VBOMesh : public Drawable
{
private:
    unsigned int faces;
    unsigned int vaoHandle;

    bool reCenterMesh, loadTex, genTang;

    void trimString( string & str );
    void storeVBO( const vector<vec3> & points,
                            const vector<vec3> & normals,
                            const vector<vec2> &texCoords,
                            const vector<vec4> &tangents,
                            const vector<int> &elements );
    void generateAveragedNormals(
            const vector<vec3> & points,
            vector<vec3> & normals,
            const vector<int> & faces );
    void generateTangents(
            const vector<vec3> & points,
            const vector<vec3> & normals,
            const vector<int> & faces,
            const vector<vec2> & texCoords,
            vector<vec4> & tangents);
    void center(vector<vec3> &);

public:
    VBOMesh( const char * fileName, bool reCenterMesh = false, bool loadTc = false, bool genTangents = false );

    void render() const;

    void loadOBJ( const char * fileName );
};

#endif // VBOMESH_H
