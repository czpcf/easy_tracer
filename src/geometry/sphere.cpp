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
    if(abs(z_axis & x_axis) > EPS) {
        using namespace std;
        cerr << "z_axis is not orthogonal to x_axis: "
             << "(" << z_axis.x << "," << z_axis.y << "," << z_axis.z << ") and "
             << "(" << x_axis.x << "," << x_axis.y << "," << x_axis.z << ")." << endl;
        assert(0);
    }
    z = z_axis.Norm();
    x = x_axis.Norm();
    y = z * x;
    ori = origin;
    r = radius;
}

std::pair<bool, std::pair<Vec2, Vec3>> Sphere::Inter(Vec3 p, Vec3 d) {
    using namespace std;
    float a = d & d;
    float b = ((p - ori) & d) * 2;
    float c = ((p - ori) & (p - ori)) - r * r;
    float del = b * b - a * c * 4;
    if(del < 0) {
        return make_pair(false, make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
    }
    float t1 = (-b - sqrt(del)) / (a * 2);
    if(t1 < 0) {
        t1 = (-b + sqrt(del)) / (a * 2);
    }
    if(t1 < 0) {
        return make_pair(false, make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
    }
    Vec3 inter = p + d * t1;
    p = inter - ori;
    float theta = p & z;
    if(theta <= -1.0 + EPS) {
        return make_pair(true, make_pair(Vec2(PI, 0.0f), inter));
    } else if(theta >= 1.0 - EPS) {
        return make_pair(true, make_pair(Vec2(0.0f, 0.0f), inter));
    }
    theta = acos(theta);
    Vec3 proj = p - z * (p & z); // be careful
    float ax = x.Angle(proj);
    float ay = y.Angle(proj);
    if(ax < PI / 2) {
        return make_pair(true, make_pair(Vec2(theta, ay), inter));
    }
    return make_pair(true, make_pair(Vec2(theta, PI * 2 - ay), inter));
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
    using namespace std;
    cerr << "Debug Sphere: ";
    cerr << '(' << ori.x << ',' << ori.y << ',' << ori.z << ')' << endl;
    cerr << "z-axis: " << '(' << z.x << ',' << z.y << ',' << z.z << ')';
    cerr << "x-axis: " << '(' << x.x << ',' << x.y << ',' << x.z << ')';
    cerr << "y-axis: " << '(' << y.x << ',' << y.y << ',' << y.z << ')';
    cerr << "radius: " << r << endl;
}

Box Sphere::Bound() {
    return Box(ori - Vec3(r, r, r), ori + Vec3(r, r, r));
}
