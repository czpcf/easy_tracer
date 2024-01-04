/*
Pinpoint camera
*/
#ifndef CAMERA_PINPOINT_H
#define CAMERA_PINPOINT_H
#include "camera.h"
#include "../tensor/mat3.h"
class CameraPinhole: public Camera {
private:
    /// @brief z-axis and x-axis, y=z*x
    Vec3 z, x, y;
    /// @brief the point of the pinhole
    Vec3 ori;
    float width;
    float height;
    float dep;
    /// @brief transformation from camera coordiante to world coordinate
    Mat3 T;
public:
    CameraPinhole();
    ~CameraPinhole();
    CameraPinhole(Vec3 z_axis, Vec3 x_axis, Vec3 origin, float width_t, float height_t, float dep_t);
    void Init(Vec3 z_axis, Vec3 x_axis, Vec3 origin, float width_t, float height_t, float dep_t);
    /// @brief return the ray given the coordinate on the screen [-1, 1]x[-1, 1],
    /// return (origin, direction), direction is normalized
    std::pair<Vec3, Vec3> CastRay(Vec2 uv);
};
#endif
