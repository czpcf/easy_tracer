/*
sphere
*/
#include "geometry.h"
#include "../tensor/vec3.h"
#include "../tensor/mat3.h"
#ifndef SPHERE_H
#define SPHERE_H
class Sphere: public Geometry {
private:
    /// @brief z-axis of the sphere. Use [0, pi]x[0, 2pi] -> S^2 mapping
    /// (sin\theta sin\phi, sin\theta cos\phi, cos\theta)
    Vec3 z;
    /// @brief x-axis of the sphere, and then y-axis is determined by z * x
    Vec3 x, y;
    /// @brief origin of the sphere
    Vec3 ori;
    /// @brief radius
    float r;
public:
    Sphere();
    ~Sphere();

    /// @brief z-axis, x-axis, origin and the radius
    Sphere(Vec3, Vec3, Vec3, float);

    /// @brief return local coordinate and the nearest intersection
    std::pair<bool, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d);
    
    /// @brief return the interpolation uv of the sphere
    Vec2 GetUV(Vec2 local);

    /// @brief return the interpolation norm of the sphere
    Vec3 GetNorm(Vec2 local);

    /// @brief apply the transform to the sphere
    void Trans(Mat3& T);
    void Debug();

    Box Bound();
};
#endif