/// base class of other shapes
/// must implement:
/// 1. Inter(vec p, vec d): intersection with the ray p + t*d,
///    need to return local coordinate and the xyz position
/// 2. Trans(Mat3& T): apply the transform to change the position,
///    direction, ... of the geometry
/// 3. Bound(): the bounding box of the geometry
/// 4. GetUVInter(Vec2 local): return uv mapping via local coordinate
/// 5. GetNormInter(Vec2 local): return interpolated normal via local coordinate
/// optional:
/// 1. Debug: just to print some debug information, it can be empty
#include <utility>
#include "../tensor/vec2.h"
#include "../tensor/vec3.h"
#include "../tensor/mat3.h"
#include "../accel/box.h"
#ifndef GEOMETRY_H
#define GEOMETRY_H
class Geometry {
public:
    Geometry();
    ~Geometry();
    /// @brief intersection with the line from point p in direction d
    /// if intersected, return <true, <local coordinate, normal>>
    /// otherwise <false, <(0, 0), (0, 0, 0)>>
    virtual std::pair<bool, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d) = 0;
    virtual void Trans(Mat3& T) = 0;
    virtual void Debug() = 0;
    virtual Box Bound() = 0;
    virtual Vec2 GetUVInter(Vec2 local) = 0;
    virtual Vec3 GetNormInter(Vec2 local) = 0;
};
#endif