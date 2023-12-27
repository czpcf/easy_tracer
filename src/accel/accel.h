/*
accelerator
*/
#ifndef ACCEL_H
#define ACCEL_H
#include <vector>
#include <utility>
#include "../geometry/geometry.h"
class Accel {
public:
    Accel();
    ~Accel();

    /// @brief initialize the acclerator with shapes
    virtual void Init(std::vector<Geometry*>& shapes) = 0;
    
    /// @brief intersection with the line from point p in direction d
    /// if intersected, return <shape id, <local coordinate, normal>>
    /// otherwise <-1, <(0, 0), (0, 0, 0)>>
    virtual std::pair<int, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d) = 0;
};
#endif
