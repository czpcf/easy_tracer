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
    /// 3 norms of the points
    Vec3 norm1, norm2, norm3;
    // norm of the triangle, determined by (p2 - p1) * (p3 - p1)
    Vec3 norm;
public:
    Triangle();
    ~Triangle();
    /// @brief counter-clockwise: p1 -> p2 -> p3, and no uv mapping
    Triangle(Vec3, Vec3, Vec3);

    /// @brief counter-clockwise: p1 -> p2 -> p3, with uv mapping
    Triangle(Vec3, Vec3, Vec3, Vec2, Vec2, Vec2);

    /// @brief counter-clockwise: p1 -> p2 -> p3, with normals
    Triangle(Vec3, Vec3, Vec3, Vec3, Vec3, Vec3);

    /// @brief counter-clockwise: p1 -> p2 -> p3, with uv mapping and normals
    Triangle(Vec3, Vec3, Vec3, Vec2, Vec2, Vec2, Vec3, Vec3, Vec3);
    
    /// @brief intersection with the line from point p in direction d
    /// if intersected, return <true, <local coordinate, intersection>>
    /// otherwise <false, <(0, 0), (0, 0, 0)>>
    std::pair<bool, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d);
    
    /// @brief return the norm of the triangle
    Vec3 GetNorm();

    /// @brief return the interpolation uv of the triangle
    Vec2 GetUVInter(Vec2 local);

    /// @brief return the interpolation norm of the triangle
    Vec3 GetNormInter(Vec2 local);

    /// @brief apply the transform to the triangle
    void Trans(Mat3& T);
    void Debug();

    Box Bound();
};
#endif