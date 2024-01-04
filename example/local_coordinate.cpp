#include <iostream>
#include "../src/tensor/vec3.h"
#include "../src/geometry/triangle.h"
using namespace std;
int main() {
    Vec3 p(0, 0, 1);
    Vec3 d(0, 0, -1);
    Triangle t(Vec3(-1, -1, 0), Vec3(1, -1, 0), Vec3(0, 1, 0));
    Vec2 uv1 = Vec2(-1, -1), uv2 = Vec2(1, -1), uv3 = Vec2(0, 1);
    auto res = t.Inter(p, d);
    Vec3 inter = res.second.second;
    Vec2 local = res.second.first;
    cout << inter.x << ' ' << inter.y << ' ' << inter.z << endl;
    cout << local.x << ' ' << local.y << endl;
    Vec2 uv = t.Interpolate(local, uv1, uv2, uv3);
    cout << uv.x << ' ' << uv.y << endl;
    return 0;
}