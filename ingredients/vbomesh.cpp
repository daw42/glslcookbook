#include "vbomesh.h"
#include "glutils.h"

#include <cstdlib>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::istringstream;

VBOMesh::VBOMesh(const char * fileName, bool center, bool loadTc, bool genTangents) :
        reCenterMesh(center), loadTex(loadTc), genTang(genTangents)
{
    loadOBJ(fileName);
}

void VBOMesh::render() const {
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 3 * faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}

void VBOMesh::loadOBJ( const char * fileName ) {

    vector <vec3> points;
    vector <vec3> normals;
    vector <vec2> texCoords;
    vector <GLuint> faces;

    int nFaces = 0;

    ifstream objStream( fileName, std::ios::in );

    if( !objStream ) {
        cerr << "Unable to open OBJ file: " << fileName << endl;
        exit(1);
    }

    string line, token;
    vector<int> face;

    getline( objStream, line );
    while( !objStream.eof() ) {
        trimString(line);
        if( line.length( ) > 0 && line.at(0) != '#' ) {
            istringstream lineStream( line );

            lineStream >> token;

            if (token == "v" ) {
                float x, y, z;
                lineStream >> x >> y >> z;
                points.push_back( vec3(x,y,z) );
            } else if (token == "vt" && loadTex) {
                // Process texture coordinate
                float s,t;
                lineStream >> s >> t;
                texCoords.push_back( vec2(s,t) );
            } else if (token == "vn" ) {
                float x, y, z;
                lineStream >> x >> y >> z;
                normals.push_back( vec3(x,y,z) );
            } else if (token == "f" ) {
                nFaces++;

                // Process face
                face.clear();
                size_t slash1, slash2;
                //int point, texCoord, normal;
                while( lineStream.good() ) {
                    string vertString;
                    lineStream >> vertString;
                    int pIndex = -1, nIndex = -1 , tcIndex = -1;

                    slash1 = vertString.find("/");
                    if( slash1 == string::npos ){
                        pIndex = atoi( vertString.c_str() ) - 1;
                    } else {
                        slash2 = vertString.find("/", slash1 + 1 );
                        pIndex = atoi( vertString.substr(0,slash1).c_str() )
                                        - 1;
                        if( slash2 > slash1 + 1 ) {
                                tcIndex =
                                        atoi( vertString.substr(slash1 + 1, slash2).c_str() )
                                        - 1;
                        }
                        nIndex =
                                atoi( vertString.substr(slash2 + 1,vertString.length()).c_str() )
                                - 1;
                    }
                    if( pIndex == -1 ) {
                        printf("Missing point index!!!");
                    } else {
                        face.push_back(pIndex);
                    }

                    if( loadTex && tcIndex != -1 && pIndex != tcIndex ) {
                        printf("Texture and point indices are not consistent.\n");
                    }
                    if ( nIndex != -1 && nIndex != pIndex ) {
                        printf("Normal and point indices are not consistent.\n");
                    }
                }
                // If number of edges in face is greater than 3,
                // decompose into triangles as a triangle fan.
                if( face.size() > 3 ) {
                    int v0 = face[0];
                    int v1 = face[1];
                    int v2 = face[2];
                    // First face
                    faces.push_back(v0);
                    faces.push_back(v1);
                    faces.push_back(v2);
                    for( GLuint i = 3; i < face.size(); i++ ) {
                        v1 = v2;
                        v2 = face[i];
                        faces.push_back(v0);
                        faces.push_back(v1);
                        faces.push_back(v2);
                    }
                } else {
                    faces.push_back(face[0]);
                    faces.push_back(face[1]);
                    faces.push_back(face[2]);
                }
            }
        }
        getline( objStream, line );
    }

    objStream.close();

    if( normals.size() == 0 ) {
        generateAveragedNormals(points,normals,faces);
    }

    vector<vec4> tangents;
    if( genTang && texCoords.size() > 0 ) {
        generateTangents(points,normals,faces,texCoords,tangents);
    }

    if( reCenterMesh ) {
        center(points);
    }

    storeVBO(points, normals, texCoords, tangents, faces);

    cout << "Loaded mesh from: " << fileName << endl;
    cout << " " << points.size() << " points" << endl;
    cout << " " << nFaces << " faces" << endl;
    cout << " " << faces.size() / 3 << " triangles." << endl;
    cout << " " << normals.size() << " normals" << endl;
    cout << " " << tangents.size() << " tangents " << endl;
    cout << " " << texCoords.size() << " texture coordinates." << endl;
}

void VBOMesh::center( vector<vec3> & points ) {
    if( points.size() < 1) return;

    vec3 maxPoint = points[0];
    vec3 minPoint = points[0];

    // Find the AABB
    for( GLuint i = 0; i < points.size(); ++i ) {
        vec3 & point = points[i];
        if( point.x > maxPoint.x ) maxPoint.x = point.x;
        if( point.y > maxPoint.y ) maxPoint.y = point.y;
        if( point.z > maxPoint.z ) maxPoint.z = point.z;
        if( point.x < minPoint.x ) minPoint.x = point.x;
        if( point.y < minPoint.y ) minPoint.y = point.y;
        if( point.z < minPoint.z ) minPoint.z = point.z;
    }

    // Center of the AABB
    vec3 center = vec3( (maxPoint.x + minPoint.x) / 2.0f,
                        (maxPoint.y + minPoint.y) / 2.0f,
                        (maxPoint.z + minPoint.z) / 2.0f );

    // Translate center of the AABB to the origin
    for( GLuint i = 0; i < points.size(); ++i ) {
        vec3 & point = points[i];
        point = point - center;
    }
}

void VBOMesh::generateAveragedNormals(
        const vector<vec3> & points,
        vector<vec3> & normals,
        const vector<GLuint> & faces )
{
    for( GLuint i = 0; i < points.size(); i++ ) {
        normals.push_back(vec3(0.0f));
    }

    for( GLuint i = 0; i < faces.size(); i += 3) {
        const vec3 & p1 = points[faces[i]];
        const vec3 & p2 = points[faces[i+1]];
        const vec3 & p3 = points[faces[i+2]];

        vec3 a = p2 - p1;
        vec3 b = p3 - p1;
        vec3 n = glm::normalize(glm::cross(a,b));

        normals[faces[i]] += n;
        normals[faces[i+1]] += n;
        normals[faces[i+2]] += n;
    }

    for( GLuint i = 0; i < normals.size(); i++ ) {
        normals[i] = glm::normalize(normals[i]);
    }
}

void VBOMesh::generateTangents(
        const vector<vec3> & points,
        const vector<vec3> & normals,
        const vector<GLuint> & faces,
        const vector<vec2> & texCoords,
        vector<vec4> & tangents)
{
    vector<vec3> tan1Accum;
    vector<vec3> tan2Accum;

    for( GLuint i = 0; i < points.size(); i++ ) {
        tan1Accum.push_back(vec3(0.0f));
        tan2Accum.push_back(vec3(0.0f));
        tangents.push_back(vec4(0.0f));
    }

    // Compute the tangent vector
    for( GLuint i = 0; i < faces.size(); i += 3 )
    {
        const vec3 &p1 = points[faces[i]];
        const vec3 &p2 = points[faces[i+1]];
        const vec3 &p3 = points[faces[i+2]];

        const vec2 &tc1 = texCoords[faces[i]];
        const vec2 &tc2 = texCoords[faces[i+1]];
        const vec2 &tc3 = texCoords[faces[i+2]];

        vec3 q1 = p2 - p1;
        vec3 q2 = p3 - p1;
        float s1 = tc2.x - tc1.x, s2 = tc3.x - tc1.x;
        float t1 = tc2.y - tc1.y, t2 = tc3.y - tc1.y;
        float r = 1.0f / (s1 * t2 - s2 * t1);
        vec3 tan1( (t2*q1.x - t1*q2.x) * r,
                   (t2*q1.y - t1*q2.y) * r,
                   (t2*q1.z - t1*q2.z) * r);
        vec3 tan2( (s1*q2.x - s2*q1.x) * r,
                   (s1*q2.y - s2*q1.y) * r,
                   (s1*q2.z - s2*q1.z) * r);
        tan1Accum[faces[i]] += tan1;
        tan1Accum[faces[i+1]] += tan1;
        tan1Accum[faces[i+2]] += tan1;
        tan2Accum[faces[i]] += tan2;
        tan2Accum[faces[i+1]] += tan2;
        tan2Accum[faces[i+2]] += tan2;
    }

    for( GLuint i = 0; i < points.size(); ++i )
    {
        const vec3 &n = normals[i];
        vec3 &t1 = tan1Accum[i];
        vec3 &t2 = tan2Accum[i];

        // Gram-Schmidt orthogonalize
        tangents[i] = vec4(glm::normalize( t1 - (glm::dot(n,t1) * n) ), 0.0f);
        // Store handedness in w
        tangents[i].w = (glm::dot( glm::cross(n,t1), t2 ) < 0.0f) ? -1.0f : 1.0f;
    }
    tan1Accum.clear();
    tan2Accum.clear();
}

void VBOMesh::storeVBO( const vector<vec3> & points,
                        const vector<vec3> & normals,
                        const vector<vec2> &texCoords,
                        const vector<vec4> &tangents,
                        const vector<GLuint> &elements )
{
    GLuint nVerts  = GLuint(points.size());
    faces = GLuint(elements.size() / 3);

    float * v = new float[3 * nVerts];
    float * n = new float[3 * nVerts];
    float * tc = NULL;
    float * tang = NULL;

    if(texCoords.size() > 0) {
        tc = new float[ 2 * nVerts];
        if( tangents.size() > 0 )
            tang = new float[4*nVerts];
    }

    unsigned int *el = new unsigned int[elements.size()];

    int idx = 0, tcIdx = 0, tangIdx = 0;
    for( GLuint i = 0; i < nVerts; ++i )
    {
        v[idx] = points[i].x;
        v[idx+1] = points[i].y;
        v[idx+2] = points[i].z;
        n[idx] = normals[i].x;
        n[idx+1] = normals[i].y;
        n[idx+2] = normals[i].z;
        idx += 3;
        if( tc != NULL ) {
            tc[tcIdx] = texCoords[i].x;
            tc[tcIdx+1] = texCoords[i].y;
            tcIdx += 2;
        }
        if( tang != NULL ) {
            tang[tangIdx] = tangents[i].x;
            tang[tangIdx+1] = tangents[i].y;
            tang[tangIdx+2] = tangents[i].z;
            tang[tangIdx+3] = tangents[i].w;
            tangIdx += 4;
        }
    }
    for( unsigned int i = 0; i < elements.size(); ++i )
    {
        el[i] = elements[i];
    }
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    int nBuffers = 3;
    if( tc != NULL ) nBuffers++;
    if( tang != NULL ) nBuffers++;
    GLuint elementBuffer = nBuffers - 1;

    GLuint handle[5];
    GLuint bufIdx = 0;
    glGenBuffers(nBuffers, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[bufIdx++]);
    glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), v, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[bufIdx++]);
    glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), n, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(1);  // Vertex normal

    if( tc != NULL ) {
        glBindBuffer(GL_ARRAY_BUFFER, handle[bufIdx++]);
        glBufferData(GL_ARRAY_BUFFER, (2 * nVerts) * sizeof(float), tc, GL_STATIC_DRAW);
        glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
        glEnableVertexAttribArray(2);  // Texture coords
    }
    if( tang != NULL ) {
        glBindBuffer(GL_ARRAY_BUFFER, handle[bufIdx++]);
        glBufferData(GL_ARRAY_BUFFER, (4 * nVerts) * sizeof(float), tang, GL_STATIC_DRAW);
        glVertexAttribPointer( (GLuint)3, 4, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
        glEnableVertexAttribArray(3);  // Tangent vector
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[elementBuffer]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * faces * sizeof(unsigned int), el, GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Clean up
    delete [] v;
    delete [] n;
    if( tc != NULL ) delete [] tc;
    if( tang != NULL ) delete [] tang;
    delete [] el;
}

void VBOMesh::trimString( string & str ) {
    const char * whiteSpace = " \t\n\r";
    size_t location;
    location = str.find_first_not_of(whiteSpace);
    str.erase(0,location);
    location = str.find_last_not_of(whiteSpace);
    str.erase(location + 1);
}
