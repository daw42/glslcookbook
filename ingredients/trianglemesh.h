#pragma once

#include <vector>

#include "cookbookogl.h"
#include "drawable.h"

class TriangleMesh : public Drawable {

protected:

    GLuint nVerts;     // Number of vertices
    GLuint vao;        // The Vertex Array Object

    // Vertex buffers
    std::vector<GLuint> buffers;

    virtual void initBuffers(
            std::vector<GLuint> * indices,
            std::vector<GLfloat> * points,
            std::vector<GLfloat> * normals,
            std::vector<GLfloat> * texCoords = nullptr,
            std::vector<GLfloat> * tangents = nullptr
            );

    virtual void deleteBuffers();

public:
    virtual ~TriangleMesh();
    virtual void render() const;
    GLuint getVao() const { return vao; }
};
