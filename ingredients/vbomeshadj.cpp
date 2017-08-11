#include "vbomeshadj.h"
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

#include <map>
using std::map;

VBOMeshAdj::VBOMeshAdj(const char * fileName, bool center)
{
    loadOBJ(fileName, center);
}

void VBOMeshAdj::render() const {
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES_ADJACENCY, 6 * faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}

void VBOMeshAdj::determineAdjacency(vector<GLuint> &el)
{
    // Elements with adjacency info
    vector<GLuint> elAdj;

    // Copy and make room for adjacency info
    for( GLuint i = 0; i < el.size(); i+=3)
    {
        elAdj.push_back(el[i]);
        elAdj.push_back(-1);
        elAdj.push_back(el[i+1]);
        elAdj.push_back(-1);
        elAdj.push_back(el[i+2]);
        elAdj.push_back(-1);
    }

    // Find matching edges
    for( GLuint i = 0; i < elAdj.size(); i+=6)
    {
        // A triangle
        int a1 = elAdj[i];
        int b1 = elAdj[i+2];
        int c1 = elAdj[i+4];

        // Scan subsequent triangles
        for(GLuint j = i+6; j < elAdj.size(); j+=6)
        {
            int a2 = elAdj[j];
            int b2 = elAdj[j+2];
            int c2 = elAdj[j+4];

            // Edge 1 == Edge 1
            if( (a1 == a2 && b1 == b2) || (a1 == b2 && b1 == a2) )
            {
                elAdj[i+1] = c2;
                elAdj[j+1] = c1;
            }
            // Edge 1 == Edge 2
            if( (a1 == b2 && b1 == c2) || (a1 == c2 && b1 == b2) )
            {
                elAdj[i+1] = a2;
                elAdj[j+3] = c1;
            }
            // Edge 1 == Edge 3
            if ( (a1 == c2 && b1 == a2) || (a1 == a2 && b1 == c2) )
            {
                elAdj[i+1] = b2;
                elAdj[j+5] = c1;
            }
            // Edge 2 == Edge 1
            if( (b1 == a2 && c1 == b2) || (b1 == b2 && c1 == a2) )
            {
                elAdj[i+3] = c2;
                elAdj[j+1] = a1;
            }
            // Edge 2 == Edge 2
            if( (b1 == b2 && c1 == c2) || (b1 == c2 && c1 == b2) )
            {
                elAdj[i+3] = a2;
                elAdj[j+3] = a1;
            }
            // Edge 2 == Edge 3
            if( (b1 == c2 && c1 == a2) || (b1 == a2 && c1 == c2) )
            {
                elAdj[i+3] = b2;
                elAdj[j+5] = a1;
            }
            // Edge 3 == Edge 1
            if( (c1 == a2 && a1 == b2) || (c1 == b2 && a1 == a2) )
            {
                elAdj[i+5] = c2;
                elAdj[j+1] = b1;
            }
            // Edge 3 == Edge 2
            if( (c1 == b2 && a1 == c2) || (c1 == c2 && a1 == b2) )
            {
                elAdj[i+5] = a2;
                elAdj[j+3] = b1;
            }
            // Edge 3 == Edge 3
            if( (c1 == c2 && a1 == a2) || (c1 == a2 && a1 == c2) )
            {
                elAdj[i+5] = b2;
                elAdj[j+5] = b1;
            }
        }
    }

    // Look for any outside edges
    for( GLuint i = 0; i < elAdj.size(); i+=6)
    {
        if( elAdj[i+1] == -1 ) elAdj[i+1] = elAdj[i+4];
        if( elAdj[i+3] == -1 ) elAdj[i+3] = elAdj[i];
        if( elAdj[i+5] == -1 ) elAdj[i+5] = elAdj[i+2];
    }

    // Copy all data back into el
    el = elAdj;
}

void VBOMeshAdj::loadOBJ( const char * fileName, bool reCenterMesh ) {

  vector <vec3> p;
  vector <vec3> n;
  vector <vec2> tc;         // Holds tex coords from OBJ file
  vector <GLuint> faces, faceTC;

  int nFaces = 0;

  ifstream objStream( fileName, std::ios::in );

  if( !objStream ) {
    cerr << "Unable to open OBJ file: " << fileName << endl;
    exit(1);
  }

  cout << "Loading OBJ mesh: " << fileName << endl;
  string line, token;

  getline( objStream, line );
  while( !objStream.eof() ) {
    trimString(line);
    if( line.length( ) > 0 && line.at(0) != '#' ) {
      istringstream lineStream( line );

      lineStream >> token;

      if (token == "v" ) {
        float x, y, z;
        lineStream >> x >> y >> z;
        p.push_back( vec3(x,y,z) );
      } else if (token == "vt" ) {
        // Process texture coordinate
        float s,t;
        lineStream >> s >> t;
        tc.push_back( vec2(s,t) );
      } else if (token == "vn" ) {
        float x, y, z;
        lineStream >> x >> y >> z;
        n.push_back( vec3(x,y,z) );
      } else if (token == "f" ) {
        nFaces++;

        // Process face
        size_t slash1, slash2;
        int faceVerts = 0;
        while( lineStream.good() ) {
          faceVerts++;
          string vertString;
          lineStream >> vertString;
          int pIndex = -1, nIndex = -1 , tcIndex = -1;

          slash1 = vertString.find("/");
          if( slash1 == string::npos ){
            pIndex = atoi( vertString.c_str() ) - 1 ;
          } else {
            slash2 = vertString.find("/", slash1 + 1 );
            pIndex = atoi( vertString.substr(0,slash1).c_str() ) - 1;
            if( slash2 == string::npos || slash2 > slash1 + 1) {
              tcIndex =
                atoi( vertString.substr(slash1 + 1, slash2).c_str() ) - 1;
            }
            if( slash2 != string::npos )
              nIndex =
                atoi( vertString.substr(slash2 + 1,string::npos).c_str() ) - 1;
          }
          if( pIndex == -1 ) {
            printf("Missing point index!!!");
          } else {
            faces.push_back(pIndex);
          }
          if( tcIndex != -1 ) faceTC.push_back(tcIndex);

          if ( nIndex != -1 && nIndex != pIndex ) {
            printf("Normal and point indices are not consistent.\n");
          }
        }
        if( faceVerts != 3 ) {
          printf("Found non-triangular face.\n");
        }
      }
    }
    getline( objStream, line );
  }

  objStream.close();

  // 2nd pass, re-do the lists to make the indices consistent
  vector<vec2> texCoords;
  for( GLuint i = 0; i < p.size(); i++ ) texCoords.push_back(vec2(0.0f));
  std::map<int, int> pToTex;
  for( GLuint i = 0; i < faces.size(); i++ ) {
    int point = faces[i];
    int texCoord = faceTC[i];
    std::map<int, int>::iterator it = pToTex.find(point);
    if( it == pToTex.end() ) {
      pToTex[point] = texCoord;
      texCoords[point] = tc[texCoord];
    } else {
      if( texCoord != it->second ) {
        p.push_back( p[point] );  // Dup the point
        texCoords.push_back( tc[texCoord] );
        faces[i] = GLuint(p.size() - 1);
      }
    }
  }

  if( n.size() == 0 ) {
    cout << "Generating normal vectors" << endl;
    generateAveragedNormals(p,n,faces);
  }

  vector<vec4> tangents;
  if( texCoords.size() > 0 ) {
    cout << "Generating tangents" << endl;
    generateTangents(p,n,faces,texCoords,tangents);
  }

  if( reCenterMesh ) {
    center(p);
  }

  // Determine the adjacency information
  cout << "Determining mesh adjacencies" << endl;
  determineAdjacency(faces);

  storeVBO(p, n, texCoords, tangents, faces);

  cout << "Loaded mesh from: " << fileName << endl;
  cout << " " << p.size() << " points" << endl;
  cout << " " << nFaces << " faces" << endl;
  cout << " " << n.size() << " normals" << endl;
  cout << " " << tangents.size() << " tangents " << endl;
  cout << " " << texCoords.size() << " texture coordinates." << endl;
}

void VBOMeshAdj::center( vector<vec3> & points ) {
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

void VBOMeshAdj::generateAveragedNormals(
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

void VBOMeshAdj::generateTangents(
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

void VBOMeshAdj::storeVBO( const vector<vec3> & points,
                        const vector<vec3> & normals,
                        const vector<vec2> &texCoords,
                        const vector<vec4> &tangents,
                        const vector<GLuint> &elements )
{
    GLuint nVerts  = GLuint(points.size());
    faces = GLuint(elements.size() / 6);

    float * v = new float[3 * nVerts];
    float * n = new float[3 * nVerts];
    float * tc = NULL;
    float * tang = NULL;

    if(texCoords.size() > 0 && tangents.size() > 0) {
        tc = new float[ 2 * nVerts];
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
            tang[tangIdx] = tangents[i].x;
            tang[tangIdx+1] = tangents[i].y;
            tang[tangIdx+2] = tangents[i].z;
            tang[tangIdx+3] = tangents[i].w;
            tangIdx += 4;
            tc[tcIdx] = texCoords[i].x;
            tc[tcIdx+1] = texCoords[i].y;
            tcIdx += 2;
        }
    }
    for( unsigned int i = 0; i < elements.size(); ++i )
    {
        el[i] = elements[i];
    }
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    int nBuffers = 5;
    GLuint elementBuffer = 4;
    if( tc == NULL ) {
        nBuffers = 3;
        elementBuffer = 2;
    }

    unsigned int handle[5];
    glGenBuffers(nBuffers, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), v, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), n, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(1);  // Vertex normal

    if( tc != NULL ) {
        glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
        glBufferData(GL_ARRAY_BUFFER, (2 * nVerts) * sizeof(float), tc, GL_STATIC_DRAW);
        glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
        glEnableVertexAttribArray(2);  // Texture coords

        glBindBuffer(GL_ARRAY_BUFFER, handle[3]);
        glBufferData(GL_ARRAY_BUFFER, (4 * nVerts) * sizeof(float), tang, GL_STATIC_DRAW);
        glVertexAttribPointer( (GLuint)3, 4, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
        glEnableVertexAttribArray(3);  // Tangent vector
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[elementBuffer]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof(unsigned int), el, GL_STATIC_DRAW);

    glBindVertexArray(0);

    delete [] v;
    delete [] n;
    if( tc != NULL ) {
        delete [] tc;
        delete [] tang;
    }
    delete [] el;
    printf("End storeVBO\n");
}

void VBOMeshAdj::trimString( string & str ) {
    const char * whiteSpace = " \t\n\r";
    size_t location;
    location = str.find_first_not_of(whiteSpace);
    str.erase(0,location);
    location = str.find_last_not_of(whiteSpace);
    str.erase(location + 1);
}
