#include <iostream>
#include "../src/tensor/vec3.h"
#include "../src/geometry/triangle.h"
using namespace std;
int main() {
    Vec3 p(1, 2, 3);
    Vec3 d(4, 5, 6);
    Triangle t(Vec3(-100000, -100000, 1000), Vec3(-100000, 100000, 1000), Vec3(1000000, 1000000, 1000));
    auto res = t.Inter(p, d);
    Vec3 inter = res.second.second;
    cout << inter.x << ' ' << inter.y << ' ' <<inter.z << endl;
    return 0;
}