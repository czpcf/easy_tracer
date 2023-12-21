#include "box.h"

Box::Box() {
}

Box::~Box() {
}

Vec3 MinVec3(const Vec3& a, const Vec3& b) {
    return Vec3(a.x < b.x ? a.x : b.x,
                a.y < b.y ? a.y : b.y,
                a.z < b.z ? a.z : b.z);
}

Vec3 MaxVec3(const Vec3& a, const Vec3& b) {
    return Vec3(a.x > b.x ? a.x : b.x,
                a.y > b.y ? a.y : b.y,
                a.z > b.z ? a.z : b.z);
}

Box::Box(Vec3 p1, Vec3 p2) {
    lower = MinVec3(p1, p2);
    upper = MaxVec3(p1, p2);
}

void Box::Add(Vec3 p) {
    lower = MinVec3(lower, p);
    upper = MaxVec3(upper, p);
}