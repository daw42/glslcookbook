#include "frustum.h"
#include <limits>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::vec3;

Frustum::Frustum() : vao(0)
{
    this->orient( vec3(0.0f,0.0f,1.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f) );
    this->setPerspective(50.0f, 1.0f, 0.5f, 100.0f);
}

Frustum::~Frustum() {
    deleteBuffers();
}

void Frustum::orient( const vec3 &pos, const vec3& at, const vec3& up )
{
    n = glm::normalize(pos - at);
    u = glm::normalize(glm::cross(up, n));
    v = glm::normalize(glm::cross(n, u));
    center = pos;
}

void Frustum::setPerspective( float fovy, float ar, float nearDist, float farDist )
{
    this->deleteBuffers();
    this->fovy = fovy;
    this->ar = ar;
    this->mNear = nearDist;
    this->mFar = farDist;

    float dy = mNear * tanf( glm::radians(fovy) / 2.0f );
    float dx = ar * dy;
    float fdy = mFar * tanf( glm::radians(fovy) / 2.0f );
    float fdx = ar * fdy;

    std::vector<GLfloat> vert = {
            0,0,0,
            dx, dy, -mNear, -dx, dy, -mNear, -dx, -dy, -mNear, dx, -dy, -mNear,
            fdx, fdy, -mFar, -fdx, fdy, -mFar, -fdx, -fdy, -mFar, fdx, -fdy, -mFar
    };

    std::vector<GLuint> el = {
            0, 5, 0, 6, 0, 7, 0, 8,
            // The near plane
            1, 2, 2, 3, 3, 4, 4, 1,
            // The far plane
            5,6,6,7,7,8,8,5
    };

    GLuint posBuf;
    glGenBuffers(1, &posBuf);
    buffers.push_back(posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glBufferData(GL_ARRAY_BUFFER, 9 * 3 * sizeof(GLfloat), vert.data(), GL_STATIC_DRAW);

    GLuint elBuf;
    glGenBuffers(1, &elBuf);
    buffers.push_back(elBuf);
    glBindBuffer(GL_ARRAY_BUFFER, elBuf);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLuint), el.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elBuf);
    glBindVertexArray(0);
}

mat4 Frustum::getViewMatrix() const {
    glm::mat4 rot(u.x, v.x, n.x, 0, u.y, v.y, n.y, 0, u.z, v.z, n.z, 0, 0, 0, 0, 1);
    glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(-center.x, -center.y, -center.z));
    return rot * trans;
}

glm::mat4 Frustum::getInverseViewMatrix() const {
    glm::mat4 rot(u.x, u.y, u.z, 0, v.x, v.y, v.z, 0, n.x, n.y, n.z, 0, 0, 0, 0, 1);
    glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(center.x, center.y, center.z));
    return trans * rot;
}

mat4 Frustum::getProjectionMatrix() const
{
    return glm::perspective( glm::radians(fovy), ar, mNear, mFar );
}

vec3 Frustum::getOrigin() const
{
    return this->center;
}

void Frustum::render() const
{
    if( vao != 0 ) {
        glBindVertexArray(vao);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Frustum::deleteBuffers() {
    if( ! buffers.empty() ) {
        glDeleteBuffers( (GLsizei)buffers.size(), buffers.data() );
        buffers.clear();
    }

    if( vao != 0 ) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}
