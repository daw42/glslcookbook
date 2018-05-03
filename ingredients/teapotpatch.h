#pragma once

#include "trianglemesh.h"
#include <glm/glm.hpp>

class TeapotPatch : public TriangleMesh
{
public:
    TeapotPatch();

    virtual void render() const;

private:
    void generatePatches(std::vector<GLfloat> & pts);
    void buildPatchReflect(int patchNum,
                           std::vector<GLfloat> & p, int &index,
                           bool reflectX, bool reflectY);
    void buildPatch(glm::vec3 patch[][4],
                    std::vector<GLfloat> & pts, int &index, glm::mat3 reflect );
    void getPatch( int patchNum, glm::vec3 patch[][4], bool reverseV );

    void initBuffers(std::vector<GLfloat> & pts);
};
