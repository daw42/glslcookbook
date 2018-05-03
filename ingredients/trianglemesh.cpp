#include "trianglemesh.h"

void TriangleMesh::initBuffers(
        std::vector<GLuint> * indices,
        std::vector<GLfloat> * points,
        std::vector<GLfloat> * normals,
        std::vector<GLfloat> * texCoords,
        std::vector<GLfloat> * tangents
) {

    // Must have data for indices, points, and normals
    if( indices == nullptr || points == nullptr || normals == nullptr )
        return;

    nVerts = (GLuint)indices->size();

    GLuint indexBuf = 0, posBuf = 0, normBuf = 0, tcBuf = 0, tangentBuf = 0;
    glGenBuffers(1, &indexBuf);
    buffers.push_back(indexBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), indices->data(), GL_STATIC_DRAW);

    glGenBuffers(1, &posBuf);
    buffers.push_back(posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glBufferData(GL_ARRAY_BUFFER, points->size() * sizeof(GLfloat), points->data(), GL_STATIC_DRAW);

    glGenBuffers(1, &normBuf);
    buffers.push_back(normBuf);
    glBindBuffer(GL_ARRAY_BUFFER, normBuf);
    glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(GLfloat), normals->data(), GL_STATIC_DRAW);

    if( texCoords != nullptr ) {
        glGenBuffers(1, &tcBuf);
        buffers.push_back(tcBuf);
        glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
        glBufferData(GL_ARRAY_BUFFER, texCoords->size() * sizeof(GLfloat), texCoords->data(), GL_STATIC_DRAW);
    }

    if( tangents != nullptr ) {
        glGenBuffers(1, &tangentBuf);
        buffers.push_back(tangentBuf);
        glBindBuffer(GL_ARRAY_BUFFER, tangentBuf);
        glBufferData(GL_ARRAY_BUFFER, tangents->size() * sizeof(GLfloat), tangents->data(), GL_STATIC_DRAW);
    }

    glGenVertexArrays( 1, &vao );
    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);

    // Position
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);  // Vertex position

    // Normal
    glBindBuffer(GL_ARRAY_BUFFER, normBuf);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(1);  // Normal

    // Tex coords
    if( texCoords != nullptr ) {
        glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);  // Tex coord
    }

    if( tangents != nullptr ) {
        glBindBuffer(GL_ARRAY_BUFFER, tangentBuf);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(3);  // Tangents
    }

    glBindVertexArray(0);
}

void TriangleMesh::render() const {
    if(vao == 0) return;

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, nVerts, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

TriangleMesh::~TriangleMesh() {
    deleteBuffers();
}

void TriangleMesh::deleteBuffers() {
    if( buffers.size() > 0 ) {
        glDeleteBuffers( (GLsizei)buffers.size(), buffers.data() );
        buffers.clear();
    }

    if( vao != 0 ) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}