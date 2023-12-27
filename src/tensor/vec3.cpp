#include <cmath>
#include "vec3.h"
#include "../const.h"

Vec3::Vec3() {
    x = y = z = 0.0f;
}

Vec3::~Vec3() {
}

Vec3::Vec3(float nx, float ny, float nz) {
    x = nx, y = ny, z = nz;
}

Vec3 Vec3::operator+(const Vec3&b) {
    return Vec3(x + b.x, y + b.y, z + b.z);
}

Vec3 Vec3::operator-(const Vec3&b) {
    return Vec3(x - b.x, y - b.y, z - b.z);
}

Vec3 Vec3::operator*(const float d) {
    return Vec3(x * d, y * d, z * d);
}

/// unsafe operator
Vec3 Vec3::operator/(const float d) {
    return Vec3(x / d, y / d, z / d);
}

void Vec3::operator+=(const Vec3&b) {
    x += b.x, y += b.y, z += b.z;
}

void Vec3::operator-=(const Vec3&b) {
    x -= b.x, y -= b.y, z -= b.z;
}

void Vec3::operator*=(const float d) {
    x *= d, y *= d, z *=d;
}

/// unsafe operator
void Vec3::operator/=(const float d) {
    x /= d, y /= d, z /=d;
}

/// return the cross product of a and b
Vec3 Vec3::Cross(const Vec3&b) {
    return Vec3(y * b.z - z * b.y,
                z * b.x - x * b.z,
                x * b.y - y * b.x);
}

Vec3 Vec3::operator*(const Vec3&b) {
    return Vec3(x * b.x, y * b.y, z * b.z);
}

float Vec3::Sum() {
    return x + y + z;
}

float Vec3::Dot(const Vec3&b) {
    return x * b.x + y * b.y + z * b.z;
}

Vec3 Vec3::Norm() {
    float s = sqrt(x * x + y * y + z * z);
    return Vec3(x / s, y / s, z / s);
}

void Vec3::Normalize() {
    float s = sqrt(x * x + y * y + z * z);
    x /= s;
    y /= s;
    z /= s;
}

float Vec3::Len() {
    return sqrt(x * x + y * y + z * z);
}

float Vec3::Angle(const Vec3&b) {
    float s = (x * b.x + y * b.y + z * b.z) / 
                sqrt(x * x + y * y + z * z) /
                sqrt(b.x * b.x + b.y * b.y + b.z * b.z);
    if(s <= -1.0 + EPS) {
        return PI;
    }
    if(s >= 1.0 - EPS) {
        return 0;
    }
    return acos(s);
}

float Vec3::Square() {
    return x * x + y * y + z * z;
}

Vec3 Vec3::Reflect(const Vec3&b) {
    float nx = b.x, ny = b.y, nz = b.z;
    // see implementation of mat3
    // a special case of ratating around an axis
    return Vec3((nx * nx * 2 - 1) * x + (nx * ny * 2) * y + (nx * nz * 2) * z,
               (ny * nx * 2) * x + (ny * ny * 2 - 1) * y + (ny * nz * 2) * z,
               (nz * nx * 2) * x + (nz * ny * 2) * y + (nz * nz * 2 - 1) * z);
}

Vec3 Vec3::Rotate(const Vec3&b, float theta) {
    // copied from mat3.cpp
    float s = sqrt(x * x + y * y + z * z);
    float nx = x / s;
    float ny = y / s;
    float nz = z / s;
    float a0 = cos(theta) + nx * nx * (1.0 - cos(theta));
    float a1 = nx * ny * (1.0 - cos(theta)) - nz * sin(theta);
    float a2 = nx * nz * (1.0 - cos(theta)) + ny * sin(theta);
    float a3 = ny * nx * (1.0 - cos(theta)) + nz * sin(theta);
    float a4 = cos(theta) + ny * ny * (1.0 - cos(theta));
    float a5 = ny * nz * (1.0 - cos(theta)) - nx * sin(theta);
    float a6 = nz * nx * (1.0 - cos(theta)) - ny * sin(theta);
    float a7 = nz * ny * (1.0 - cos(theta)) + nx * sin(theta);
    float a8 = cos(theta) + nz * nz * (1.0 - cos(theta));
    return Vec3(a0 * b.x + a1 * b.y + a2 * b.z,
                a3 * b.x + a4 * b.y + a5 * b.z,
                a6 * b.x + a7 * b.y + a8 * b.z);
}
