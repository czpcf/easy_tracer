#include <iostream>
#include "../src/tensor/vec3.h"
#include "../src/geometry/sphere.h"
using namespace std;
int main() {
    Sphere O(Vec3(1, 0, 1), Vec3(1, 1, -1), Vec3(0, 2, 0), 1);
    Vec3 p(0, 0, 0);
    Vec3 d(0.1, 1, 0.1);
    auto res = O.Inter(p, d);
    Vec2 local = res.second.first;
    Vec3 inter = res.second.second;
    cout << "local coordinate: " << local.x << ' ' << local.y << endl;
    cout << "intersection: " << inter.x << ' ' << inter.y << ' ' << inter.z << endl;
    Vec2 uv = O.GetUVInter(local);
    Vec3 norm = O.GetNormInter(local);
    cout << "uv coordinate: " << uv.x << ' ' << uv.y << endl;
    cout << "normal: " << norm.x << ' ' << norm.y << ' ' << norm.z << endl;
    return 0;
}
