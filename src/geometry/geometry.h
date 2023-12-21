/// base class of other shapes
/// must implement:
/// 1. Inter(vec p, vec d): intersection with the ray p + t*d,
///    need to return uv position and the xyz position
/// 2. Trans(Mat3& T): apply the transform to change the position,
///    direction, ... of the geometry
/// optional:
/// 1. Debug: just to print some debug information
#include <utility>
#include "../tensor/vec2.h"
#include "../tensor/vec3.h"
#include "../tensor/mat3.h"
#ifndef GEOMETRY_H
#define GEOMETRY_H
class Geometry {
public:
    Geometry();
    ~Geometry();
    /// @brief intersection with the line from point p in direction d
    /// if intersected, return <true, <intersection, normal>>
    /// otherwise <false, <(0, 0), (0, 0, 0)>>
    virtual std::pair<bool, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d) = 0;
    virtual void Trans(Mat3& T) = 0;
    virtual void Debug() = 0;
};
#endif