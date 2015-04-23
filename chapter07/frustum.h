#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Projection {
    enum ProjType {
        PERSPECTIVE, ORTHO
    };
}

class Frustum
{
private:
    Projection::ProjType type;

    vec3 origin;
    vec3 at;
    vec3 up;

    float mNear;
    float mFar;
    float xmin, xmax, ymin, ymax;
    float fovy, ar;

    mat4 view, proj;
    GLuint handle[2];

public:
    Frustum( Projection::ProjType type );

    void orient( const vec3 &pos, const vec3& a, const vec3& u );
    void setOrthoBounds( float xmin, float xmax, float ymin, float ymax,
                         float , float  );
    void setPerspective( float , float , float , float  );
    void enclose( const Frustum & );

    mat4 getViewMatrix() const;
    mat4 getProjectionMatrix() const;
    vec3 getOrigin() const;
    vec3 getCenter() const;

    void printInfo() const;
    void render() const;
};

#endif // FRUSTUM_H
