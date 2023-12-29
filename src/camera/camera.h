/*
Camera
*/
#ifndef CAMERA_H
#define CAMERA_H
#include <utility>
#include "../tensor/vec2.h"
#include "../tensor/vec3.h"
class Camera {
public:
    Camera();
    ~Camera();
    /// @brief return the ray given the coordinate on the screen [-1, 1]x[-1, 1],
    /// doesn't need to be one-to-one
    virtual std::pair<Vec3, Vec3> CastRay(Vec2 uv) = 0;
};
#endif
