#pragma once

#include "drawable.h"
#include "trianglemesh.h"

class Cube : public TriangleMesh
{
public:
    Cube(GLfloat size = 1.0f);
};
