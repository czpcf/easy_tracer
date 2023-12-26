#include <iostream>
#include <utility>
#include <cmath>
#include "../const.h"
#include "triangle.h"

Triangle::Triangle() {
}

Triangle::~Triangle() {
}

Triangle::Triangle(Vec3 point1, Vec3 point2, Vec3 point3) {
    /// counter-clockwise: p1 -> p2 -> p3
    p1 = point1, p2 = point2, p3 = point3;
    uv1 = uv2 = uv3 = Vec2(0, 0);
    norm1 = norm2 = norm3 = norm = ((p2 - p1) * (p3 - p1)).Norm();
}

Triangle::Triangle(Vec3 point1, Vec3 point2, Vec3 point3, Vec2 uv_1, Vec2 uv_2, Vec2 uv_3) {
    /// counter-clockwise: p1 -> p2 -> p3
    p1 = point1, p2 = point2, p3 = point3;
    uv1 = uv_1, uv2 = uv_2, uv3 = uv_3;
    norm1 = norm2 = norm3 = norm = ((p2 - p1) * (p3 - p1)).Norm();
}

Triangle::Triangle(Vec3 point1, Vec3 point2, Vec3 point3, Vec3 norm_1, Vec3 norm_2, Vec3 norm_3) {
    /// counter-clockwise: p1 -> p2 -> p3
    p1 = point1, p2 = point2, p3 = point3;
    uv1 = uv2 = uv3 = Vec2(0, 0);
    norm1 = norm_1.Norm(), norm2 = norm_2.Norm(), norm3 = norm_3.Norm();
    norm = ((p2 - p1) * (p3 - p1)).Norm();
}

Triangle::Triangle(Vec3 point1, Vec3 point2, Vec3 point3, Vec2 uv_1, Vec2 uv_2, Vec2 uv_3, Vec3 norm_1, Vec3 norm_2, Vec3 norm_3) {
    /// counter-clockwise: p1 -> p2 -> p3
    p1 = point1, p2 = point2, p3 = point3;
    uv1 = uv_1, uv2 = uv_2, uv3 = uv_3;
    norm1 = norm_1.Norm(), norm2 = norm_2.Norm(), norm3 = norm_3.Norm();
    norm = ((p2 - p1) * (p3 - p1)).Norm();
}

std::pair<bool, std::pair<Vec2, Vec3>> Triangle::Inter(Vec3 p, Vec3 d) {
    using namespace std;
    float t = d & (norm);
    if(abs(t) < EPS) {
        return make_pair(false, make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
    }
    t = ((p1 - p) & (norm)) / t;
    if(t < 0) {
        return make_pair(false, make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
    }
    Vec3 inter = p + d * t;
    float x = inter.x, y = inter.y, z = inter.z;
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;
    float x3 = p3.x, y3 = p3.y;
    float dx12 = x2 - x1, dy12 = y2 - y1;
    float dx13 = x3 - x1, dy13 = y3 - y1;
    // if in x-z plane or y-z plane
    if(abs(dx12 * dy13 - dx13 * dy12) < EPS) {
        float z1 = p1.z;
        float z2 = p2.z;
        float z3 = p3.z;
        float dz12 = z2 - z1;
        float dz13 = z3 - z1;
        // if in y-z plane
        if(abs(dx12 * dz13 - dx13 * dz12) < EPS) {
            float s1 = (y1 - y) * (z2 - z) - (y2 - y) * (z1 - z);
            float s2 = (y2 - y) * (z3 - z) - (y3 - y) * (z2 - z);
            float s3 = (y3 - y) * (z1 - z) - (y1 - y) * (z3 - z);
            if((s1 <= 0 && s2 <= 0 && s3 <=0) || (s1 >= 0 && s2 >= 0 && s3 >= 0)) {
                float sum = s1 + s2 + s3;
                float t1 = s1 / sum;
                float t3 = s3 / sum;
                // think: why?
                return make_pair(true, make_pair(Vec2(t1, t3),  inter));
            }
            return make_pair(false, make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
        }
        float s1 = (x1 - x) * (z2 - z) - (x2 - x) * (z1 - z);
        float s2 = (x2 - x) * (z3 - z) - (x3 - x) * (z2 - z);
        float s3 = (x3 - x) * (z1 - z) - (x1 - x) * (z3 - z);
        if((s1 <= 0 && s2 <= 0 && s3 <=0) || (s1 >= 0 && s2 >= 0 && s3 >= 0)) {
            float sum = s1 + s2 + s3;
            float t1 = s1 / sum;
            float t3 = s3 / sum;
            return make_pair(true, make_pair(Vec2(t1, t3),  inter));
        }
        return make_pair(false, make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
    }
    float s1 = (x1 - x) * (y2 - y) - (x2 - x) * (y1 - y);
    float s2 = (x2 - x) * (y3 - y) - (x3 - x) * (y2 - y);
    float s3 = (x3 - x) * (y1 - y) - (x1 - x) * (y3 - y);
    if((s1 <= 0 && s2 <= 0 && s3 <=0) || (s1 >= 0 && s2 >= 0 && s3 >= 0)) {
        float sum = s1 + s2 + s3;
        float t1 = s1 / sum;
        float t3 = s3 / sum;
        return make_pair(true, make_pair(Vec2(t1, t3),  inter));
    }
    return make_pair(false, make_pair(Vec2(0, 0), Vec3(0, 0, 0)));
}

Vec3 Triangle::GetNorm() {
    return norm;
}

Vec2 Triangle::GetUVInter(Vec2 local) {
    float t1 = local.x;
    float t3 = local.y;
    float t2 = 1.0f - t1 - t3;
    return uv3 * t1 + uv1 * t2 + uv2 * t3;
}

Vec3 Triangle::GetNormInter(Vec2 local) {
    float t1 = local.x;
    float t3 = local.y;
    float t2 = 1.0f - t1 - t3;
    return norm3 * t1 + norm1 * t2 + norm2 * t3;
}

void Triangle::Trans(Mat3&T) {
    p1 = T.Map(p1);
    p2 = T.Map(p2);
    p3 = T.Map(p3);
    norm1 = T.MapA(norm1); // be careful
    norm2 = T.MapA(norm2);
    norm3 = T.MapA(norm3);
    norm = ((p2 - p1) * (p3 - p1)).Norm();
}

void Triangle::Debug() {
    using namespace std;
    cerr << "Debug Triangle: ";
    cerr << '(' << p1.x << ',' << p1.y << ',' << p1.z << ')';
    cerr << '(' << p2.x << ',' << p2.y << ',' << p2.z << ')';
    cerr << '(' << p3.x << ',' << p3.y << ',' << p3.z << ')' << endl;
    cerr << "          Norm: " << norm.x << ' ' << norm.y << ' ' << norm.z << endl;
}

Box Triangle::Bound() {
    Box b(p1, p2);
    b.Add(p3);
    return b;
}