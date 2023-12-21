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
    /// 3 vertices of the uv mapping
    Vec2 uv1, uv2, uv3;
    // norm of the triangle, determined by (p2 - p1) * (p3 - p1)
    Vec3 norm;
public:
    Triangle();
    ~Triangle();
    /// @brief counter-clockwise: p1 -> p2 -> p3, and no uv mapping
    Triangle(Vec3, Vec3, Vec3);

    /// @brief counter-clockwise: p1 -> p2 -> p3
    Triangle(Vec3, Vec3, Vec3, Vec2, Vec2, Vec2);
    
    /// @brief intersection with the line from point p in direction d
    /// if intersected, return <true, <intersection, normal>>
    /// otherwise <false, <(0, 0), (0, 0, 0)>>
    std::pair<bool, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d);
    
    /// @brief return the norm of the triangle
    Vec3 GetNorm();

    /// @brief apply the transform to the triangle
    void Trans(Mat3& T);
    void Debug();
};
#endif