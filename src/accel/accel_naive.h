/*
naive accelerator
just search all the geometries
*/
#ifndef ACCEL_NAIVE_H
#define ACCEL_NAIVE_H
#include "accel.h"
class AccelNaive: public Accel {
private:
    std::vector<Geometry*> shapes;
public:
    AccelNaive();
    ~AccelNaive();

    /// @brief initialize the acclerator with shapes
    void Init(std::vector<Geometry*>& shapes);
    
    /// @brief intersection with the line from point p in direction d
    /// if intersected, return <shape id, <local coordinate, normal>>
    /// otherwise <-1, <(0, 0), (0, 0, 0)>>
    std::pair<int, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d);
};
#endif
