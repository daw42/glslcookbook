#pragma once

#include "trianglemesh.h"
#include "cookbookogl.h"

class Torus : public TriangleMesh
{
public:
    Torus(GLfloat outerRadius, GLfloat innerRadius, GLuint nsides, GLuint nrings);
};
