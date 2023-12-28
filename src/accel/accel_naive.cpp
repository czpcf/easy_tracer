#include "accel_naive.h"
AccelNaive::AccelNaive() {
}

AccelNaive::~AccelNaive() {
}

void AccelNaive::Init(std::vector<Geometry*>& shapes_g) {
    for(int i = 0; i < shapes_g.size(); ++i) {
        shapes.push_back(shapes_g[i]);
    }
}

std::pair<int, std::pair<Vec2, Vec3>> AccelNaive::Inter(Vec3 p, Vec3 d) {
    using namespace std;
    int id = -1;
    float dis;
    Vec2 uv;
    Vec3 inter;
    for(int i = 0; i < shapes.size(); ++i) {
        auto res = shapes[i]->Inter(p, d);
        if(res.first) {
            Vec3 inter2 = res.second.second;
            float dis2 = (inter2 - p).Square();
            if(id == -1 || dis2 < dis) {
                id = i;
                dis = dis2;
                uv = res.second.first; // dont forget to change uv
                inter = inter2;
            }
        }
    }
    return make_pair(id, make_pair(uv, inter));
}
