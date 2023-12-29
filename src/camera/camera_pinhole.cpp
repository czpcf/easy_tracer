#include "camera_pinhole.h"

CameraPinhole::CameraPinhole() {
}

CameraPinhole::~CameraPinhole() {
}

CameraPinhole::CameraPinhole(Vec3 z_axis, Vec3 x_axis, Vec3 origin, float width_t, float height_t, float dep_t) {
    z = z_axis;
    x = x_axis;
    z.Normalize();
    x.Normalize();
    y = z * x;
    ori = origin;
    T = TransZXP(z, x, ori);
    width = width_t / 2;
    height = height_t / 2;
    dep = dep_t;
}

void CameraPinhole::Init(Vec3 z_axis, Vec3 x_axis, Vec3 origin, float width_t, float height_t, float dep_t) {
    z = z_axis;
    x = x_axis;
    z.Normalize();
    x.Normalize();
    y = z * x;
    ori = origin;
    T = TransZXP(z, x, ori);
    width = width_t / 2;
    height = height_t / 2;
    dep = dep_t;
}

std::pair<Vec3, Vec3> CameraPinhole::CastRay(Vec2 uv) {
    Vec3 p(0.0f, 0.0f, 0.0f);
    Vec3 d(-uv.x * width, dep, -uv.y * height);
    return std::make_pair(T.Map(p), T.MapA(d).Norm());
}
