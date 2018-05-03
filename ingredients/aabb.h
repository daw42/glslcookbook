#pragma once

#include <glm/glm.hpp>
#include <limits>

// Axis-aligned bounding box
class Aabb {

public:
    glm::vec3 min, max;

    Aabb() {
        reset();
    }

    Aabb( const glm::vec3 & pt ) {
        reset();
        add(pt);
    }

    void add( glm::vec3 & pt ) {
        min.x = std::fmin(min.x, pt.x);
        min.y = std::fmin(min.y, pt.y);
        min.z = std::fmin(min.z, pt.z);

        max.x = std::fmax(max.x, pt.x);
        max.y = std::fmax(max.y, pt.y);
        max.z = std::fmax(max.z, pt.z);
    }

    void add(const Aabb & other) {
        min.x = std::fmin(min.x, other.min.x);
        min.y = std::fmin(min.y, other.min.y);
        min.z = std::fmin(min.z, other.min.z);

        max.x = std::fmax(max.x, other.max.x);
        max.y = std::fmax(max.y, other.max.y);
        max.z = std::fmax(max.z, other.max.z);
    }

    void reset() {
        min = glm::vec3( std::numeric_limits<float>::max());
        max = glm::vec3(-std::numeric_limits<float>::max());
    }

    glm::vec3 diagonal() {
        return max - min;
    }
};