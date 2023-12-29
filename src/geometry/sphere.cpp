#include <iostream>
#include <utility>
#include <cmath>
#include "../const.h"
#include "sphere.h"
#include <assert.h>

Sphere::Sphere() {
}

Sphere::~Sphere() {
}

Sphere::Sphere(Vec3 z_axis, Vec3 x_axis, Vec3 origin, float radius) {
    if(std::abs(z_axis.Dot(x_axis)) > EPS) {
        std::cerr << "z_axis is not orthogonal to x_axis: "
                  << "(" << z_axis.x << "," << z_axis.y << "," << z_axis.z << ") and "
                  << "(" << x_axis.x << "," << x_axis.y << "," << x_axis.z << ")." << std::endl;
        assert(0);
    }
    z = z_axis.Norm();
    x = x_axis.Norm();
    y = z.Cross(x);
    ori = origin;
    r = radius;
}

std::pair<bool, std::pair<Vec2, Vec3>> Sphere::Inter(Vec3 p, Vec3 d) {
    float a = d.Dot(d);
    float b = ((p - ori).Dot(d)) * 2;
    float c = ((p - ori).Dot(p - ori)) - r * r;
    float del = b * b - a * c * 4;
    if(del < 0) {
        return std::make_pair(false, std::make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
    }
    float t1 = (-b - std::sqrt(del)) / (a * 2);
    if(t1 < 0) {
        t1 = (-b + std::sqrt(del)) / (a * 2);
    }
    if(t1 < 0) {
        return std::make_pair(false, std::make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
    }
    Vec3 inter = p + d * t1;
    p = inter - ori;
    float theta = p.Dot(z) / r; // remember to divide by r because r may not be 1
    if(theta <= -1.0 + EPS) {
        return std::make_pair(true, std::make_pair(Vec2(PI, 0.0f), inter));
    } else if(theta >= 1.0 - EPS) {
        return std::make_pair(true, std::make_pair(Vec2(0.0f, 0.0f), inter));
    }
    theta = std::acos(theta);
    Vec3 proj = p - z * p.Dot(z); // be careful
    float ax = x.Angle(proj);
    float ay = y.Angle(proj);
    if(ax < PI / 2) {
        return std::make_pair(true, std::make_pair(Vec2(theta, ay), inter));
    }
    return std::make_pair(true, std::make_pair(Vec2(theta, PI * 2 - ay), inter));
}

Vec2 Sphere::GetUVInter(Vec2 local) {
    return Vec2(local.x / PI, local.y / PI / 2);
}

Vec3 Sphere::GetNormInter(Vec2 local) {
    float t1 = local.x;
    float t3 = local.y;
    Vec3 p = x.Rotate(z, -t1);
    p = z.Rotate(p, -t3);
    return p;
}

void Sphere::Trans(Mat3& T) {
    z = T.MapA(z);
    x = T.MapA(x);
    y = T.MapA(y);
    ori = T.Map(ori);
}

void Sphere::Debug() {
    std::cerr << "Debug Sphere: ";
    std::cerr << '(' << ori.x << ',' << ori.y << ',' << ori.z << ')' << std::endl;
    std::cerr << "z-axis: " << '(' << z.x << ',' << z.y << ',' << z.z << ')';
    std::cerr << "x-axis: " << '(' << x.x << ',' << x.y << ',' << x.z << ')';
    std::cerr << "y-axis: " << '(' << y.x << ',' << y.y << ',' << y.z << ')';
    std::cerr << "radius: " << r << std::endl;
}

Box Sphere::Bound() {
    return Box(ori - Vec3(r, r, r), ori + Vec3(r, r, r));
}
