/*
triangle
*/
#include "geometry.h"
#include "../tensor/vec3.h"
#include "../tensor/mat3.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H
class Triangle: public Geometry {
private:
    /// 3 vertices of the triangle
    Vec3 p1, p2, p3;
    /// norm of the triangle, determined by (p2 - p1) * (p3 - p1)
    Vec3 norm;
public:
    Triangle();
    ~Triangle();
    /// @brief counter-clockwise: p1 -> p2 -> p3, and no uv mapping
    Triangle(Vec3, Vec3, Vec3);
    
    /// @brief intersection with the line from point p in direction d
    /// if intersected, return <true, <local coordinate, intersection>>
    /// otherwise <false, <(0, 0), (0, 0, 0)>>
    std::pair<bool, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d);
    
    /// @brief return the norm of the triangle
    Vec3 GetNorm();

    /// @brief return the interpolation of three 2d points
    Vec2 Interpolate(Vec2 local, Vec2 uv1, Vec2 uv2, Vec2 uv3);

    /// @brief return the interpolation norm of three 3d points
    Vec3 Interpolate(Vec2 local, Vec3 p1, Vec3 p2, Vec3 p3);

    /// @brief apply the transform to the triangle
    void Trans(Mat3& T);
    void Debug();

    Box Bound();
};
#endif