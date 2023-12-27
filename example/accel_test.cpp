#include <iostream>
#include <random>
#include <chrono>
#include <assert.h>
#include "../src/const.h"
#include "../src/tensor/vec3.h"
#include "../src/geometry/triangle.h"
#include "../src/geometry/sphere.h"
#include "../src/accel/accel_naive.h"
#include "../src/accel/accel_kdtree.h"
using namespace std;
vector<Triangle> triangles;
vector<Sphere> spheres;
vector<Geometry*> shapes;
AccelNaive accel_naive;
AccelKDtree accel_kdtree;

mt19937 RD(123);
float R() {
    return (float)((RD() % 2147483648) - 1073741824) / 1073741824;
}
// corner case:
//       *
//     * |
//   * | |
// * | | |
//   * | |
//     * |
//       *
void Init() {
    for(int x = -100; x <= 99; ++x) {
        for(int y = -100; y <= 99; ++y) {
            Vec3 p1(x, y, -100);
            Vec3 p2(x + 1, y, -100);
            Vec3 p3(x + 1, y + 1, -100);
            triangles.push_back(Triangle(p1, p2, p3));
            p1 = Vec3(x, y, -100);
            p2 = Vec3(x + 1, y + 1, -100);
            p3 = Vec3(x, y + 1, -100);
            triangles.push_back(Triangle(p1, p2, p3));
            p1 = Vec3(x, y, 100);
            p2 = Vec3(x + 1, y, 100);
            p3 = Vec3(x + 1, y + 1, 100);
            triangles.push_back(Triangle(p1, p2, p3));
            p1 = Vec3(x, y, 100);
            p2 = Vec3(x + 1, y + 1, 100);
            p3 = Vec3(x, y + 1, 100);
            triangles.push_back(Triangle(p1, p2, p3));
        }
    }
    for(int i = 1; i <= 100; ++i) {
        Vec3 o(R() * 100, R() * 100, R() * 100);
        spheres.push_back(Sphere(Vec3(0, 0, 1), Vec3(1, 0, 0), o, R() + 1.1));
    }
    for(int i = 0; i < triangles.size(); ++i) {
        shapes.push_back(&triangles[i]);
    }
    for(int i = 0; i < spheres.size(); ++i) {
        shapes.push_back(&spheres[i]);
    }
    accel_naive.Init(shapes);
    accel_kdtree.Init(shapes);
    
}
int main() {
    Init();
    cout << "Accelerator initialized" << endl;
    Vec3 p(0, -10, 0);
    const int count = 1000;
    
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    start = chrono::high_resolution_clock::now();
    for(int i = 0; i < count; ++i) {
        float theta = (R() + 1) * PI / 2;
        float phi = (R() + 1) * PI;
        float x = sin(theta) * sin(phi);
        float y = sin(theta) * cos(phi);
        float z = cos(theta);
        auto res = accel_kdtree.Inter(p, Vec3(x, y, z));
        Vec3 inter = res.second.second;
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "KDtree: " << (double)(duration.count()) / 1000 << " ms, " <<
         (double)(duration.count()) / count << " us per query" << endl;
    
    start = chrono::high_resolution_clock::now();
    for(int i = 0; i < count; ++i) {
        float theta = (R() + 1) * PI / 2;
        float phi = (R() + 1) * PI;
        float x = sin(theta) * sin(phi);
        float y = sin(theta) * cos(phi);
        float z = cos(theta);
        auto res = accel_naive.Inter(p, Vec3(x, y, z));
        Vec3 inter = res.second.second;
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Naive: " << (double)(duration.count()) / 1000 << " ms, " <<
         (double)(duration.count()) / count << " us per query" << endl;
    

    for(int i = 0; i < count; ++i) {
        float theta = (R() + 1) * PI / 2;
        float phi = (R() + 1) * PI;
        float x = sin(theta) * sin(phi);
        float y = sin(theta) * cos(phi);
        float z = cos(theta);
        auto res1 = accel_kdtree.Inter(p, Vec3(x, y, z));
        Vec3 inter1 = res1.second.second;
        auto res2 = accel_naive.Inter(p, Vec3(x, y, z));
        Vec3 inter2 = res1.second.second;
        assert(abs(inter1.x - inter2.x) <= EPS &&
               abs(inter1.y - inter2.y) <= EPS &&
               abs(inter1.z - inter2.z) <= EPS);
    }
    return 0;
}
