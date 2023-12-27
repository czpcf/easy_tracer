#include <cmath>
#include "vec4.h"
#include "../const.h"

Vec4::Vec4() {
    x = y = z = w = 0.0f;
}

Vec4::~Vec4() {
}

Vec4::Vec4(float nx, float ny, float nz, float nw) {
    x = nx, y = ny, z = nz, w = nw;
}

Vec4 Vec4::operator+(const Vec4&b) {
    return Vec4(x + b.x, y + b.y, z + b.z, w + b.w);
}

Vec4 Vec4::operator-(const Vec4&b) {
    return Vec4(x - b.x, y - b.y, z - b.z, w - b.w);
}

Vec4 Vec4::operator*(const float d) {
    return Vec4(x * d, y * d, z * d, w * d);
}

/// unsafe operator
Vec4 Vec4::operator/(const float d) {
    return Vec4(x / d, y / d, z / d, w / d);
}

void Vec4::operator+=(const Vec4&b) {
    x += b.x, y += b.y, z += b.z, w += b.w;
}

void Vec4::operator-=(const Vec4&b) {
    x -= b.x, y -= b.y, z -= b.z, w -= b.w;
}

void Vec4::operator*=(const float d) {
    x *= d, y *= d, z *= d, w *= d;
}

/// unsafe operator
void Vec4::operator/=(const float d) {
    x /= d, y /= d, z /= d, w /= d;
}

Vec4 Vec4::operator*(const Vec4&b) {
    return Vec4(x * b.x, y * b.y, z * b.z, w * b.w);
}

float Vec4::Sum() {
    return x + y + z + w;
}

float Vec4::Dot(const Vec4&b) {
    return x * b.x + y * b.y + z * b.z + w * b.w;
}

Vec4 Vec4::Norm() {
    float s = sqrt(x * x + y * y + z * z + w * w);
    return Vec4(x / s, y / s, z / s, w / s);
}

void Vec4::Normalize() {
    float s = sqrt(x * x + y * y + z * z + w * w);
    x /= s;
    y /= s;
    z /= s;
    w /= w;
}

float Vec4::Len() {
    return sqrt(x * x + y * y + z * z + w * w);
}

float Vec4::Angle(const Vec4&b) {
    float s = (x * b.x + y * b.y + z * b.z + w * b.w) / 
                sqrt(x * x + y * y + z * z + w * w) /
                sqrt(b.x * b.x + b.y * b.y + b.z * b.z + b.w * b.w);
    if(s <= -1.0 + EPS) {
        return PI;
    }
    if(s >= 1.0 - EPS) {
        return 0;
    }
    return acos(s);
}

float Vec4::Square() {
    return x * x + y * y + z * z + w * w;
}
