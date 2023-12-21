#include <cmath>
#include "vec2.h"
#include "../const.h"

Vec2::Vec2() {
}

Vec2::~Vec2() {
}

Vec2::Vec2(float nx, float ny) {
    x = nx, y = ny;
}

Vec2 Vec2::operator+(const Vec2&b) {
    return Vec2(x + b.x, y + b.y);
}

Vec2 Vec2::operator-(const Vec2&b) {
    return Vec2(x - b.x, y - b.y);
}

Vec2 Vec2::operator*(const float d) {
    return Vec2(x * d, y * d);
}

/// unsafe operator
Vec2 Vec2::operator/(const float d) {
    return Vec2(x / d, y / d);
}

void Vec2::operator+=(const Vec2&b) {
    x += b.x, y += b.y;
}

void Vec2::operator-=(const Vec2&b) {
    x -= b.x, y -= b.y;
}

void Vec2::operator*=(const float d) {
    x *= d, y *= d;
}

/// unsafe operator
void Vec2::operator/=(const float d) {
    x /= d, y /= d;
}

/// return the dot product of a and b
float Vec2::operator&(const Vec2&b) {
    return x * b.x + y * b.y;
}

Vec2 Vec2::Norm() {
    float s = sqrt(x * x + y * y);
    return Vec2(x / s, y / s);
}

void Vec2::Normalize() {
    float s = sqrt(x * x + y * y);
    x /= s;
    y /= s;
}

float Vec2::Len() {
    return sqrt(x * x + y * y);
}

float Vec2::Angle(const Vec2&b) {
    float s = (x * b.x + y * b.y) / 
                sqrt(x * x + y * y) /
                sqrt(b.x * b.x + b.y * b.y);
    if(s <= -1.0 + EPS) {
        return PI;
    }
    if(s >= 1.0 - EPS) {
        return 0;
    }
    return acos(s);
}

float Vec2::Square() {
    return x * x + y * y;
}
