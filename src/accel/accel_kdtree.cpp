#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include "accel_kdtree.h"
AccelKDtree::AccelKDtree() {
}

AccelKDtree::~AccelKDtree() {
    if(boxes != nullptr) {
        delete[] boxes;
    }
    if(ls != nullptr) {
        delete[] ls;
    }
    if(rs != nullptr) {
        delete[] rs;
    }
}

// [l, r)
// TODO: faster, better
void AccelKDtree::Build(int l, int r, int& num) {
    if(l >= r) {
        return;
    } else if(l + 1 == r) {
        num = aux[l].second;
        boxes[num] = shapes[num]->Bound();
        return;
    }
    Vec3 mean, var;
    for(int i = l; i < r; ++i) {
        Box b = aux[i].first->Bound();
        mean = mean + (b.GetLower() + b.GetUpper()) / 2;
        var = var + (b.GetLower() + b.GetUpper()) * (b.GetLower() + b.GetUpper()) / 4;
    }
    mean = mean / (r - l);
    var = var / (r - l) - mean * mean;
    int mid = (l + r) >> 1;
    if(var.x >= var.y && var.x >= var.z) {
        auto cmp = [](std::pair<Geometry* ,int> a, std::pair<Geometry* ,int> b) {
            Box ba = a.first->Bound();
            Box bb = b.first->Bound();
            return ((ba.GetLower() + ba.GetUpper()) / 2).x <
                   ((bb.GetLower() + bb.GetUpper()) / 2).x;
        };
        std::nth_element(aux.begin() + l, aux.begin() + mid, aux.begin() + r, cmp);
    } else if(var.y >= var.x && var.y >= var.z) {
        auto cmp = [](std::pair<Geometry* ,int> a, std::pair<Geometry* ,int> b) {
            Box ba = a.first->Bound();
            Box bb = b.first->Bound();
            return ((ba.GetLower() + ba.GetUpper()) / 2).y <
                   ((bb.GetLower() + bb.GetUpper()) / 2).y;
        };
        std::nth_element(aux.begin() + l, aux.begin() + mid, aux.begin() + r, cmp);
    } else {
        auto cmp = [](std::pair<Geometry* ,int> a, std::pair<Geometry* ,int> b) {
            Box ba = a.first->Bound();
            Box bb = b.first->Bound();
            return ((ba.GetLower() + ba.GetUpper()) / 2).z <
                   ((bb.GetLower() + bb.GetUpper()) / 2).z;
        };
        std::nth_element(aux.begin() + l, aux.begin() + mid, aux.begin() + r, cmp);
    }
    num = aux[mid].second; // id
    boxes[num] = aux[mid].first->Bound();
    Build(l, mid, ls[num]);
    Build(mid + 1, r, rs[num]);
    if(ls[num] != -1) {
        boxes[num].Add(boxes[ls[num]]);
    }
    if(rs[num] != -1) {
        boxes[num].Add(boxes[rs[num]]);
    }
}

// make sure d is normalized
void AccelKDtree::Query(const Vec3& p, const Vec3& d, int num, float& dis, int& id, std::pair<Vec2, Vec3>& res) {
    if(num == -1) {
        return;
    }
    if(boxes[num].Inside(p) == false) {
        float ndis = boxes[num].InterSquaredNormed(p, d);
        if(ndis >= dis) {
            return;
        }
    }
    auto res0 = shapes[num]->Inter(p, d);
    if(res0.first) {
        Vec3 ninter = res0.second.second;
        float ndis = (ninter - p).Square();
        if(ndis < dis) {
            res = res0.second;
            dis = ndis;
            id = num;
        }
    }
    if(ls[num] == -1 && rs[num] == -1) {
        return;
    }
    if(rs[num] == -1) { // be careful with your left/right sons
        Query(p, d, ls[num], dis, id, res);
    } else if(ls[num] == -1) {
        Query(p, d, rs[num], dis, id, res);
    } else {
        Vec3 pl = (boxes[ls[num]].GetLower() + boxes[ls[num]].GetUpper()) / 2;
        Vec3 pr = (boxes[rs[num]].GetLower() + boxes[rs[num]].GetUpper()) / 2;
        if((pl - p).Dot(d) < (pr - p).Dot(d)) {
            Query(p, d, ls[num], dis, id, res);
            Query(p, d, rs[num], dis, id, res);
        } else {
            Query(p, d, rs[num], dis, id, res);
            Query(p, d, ls[num], dis, id, res);
        }
    }
}

void AccelKDtree::Init(std::vector<Geometry*>& shapes_g) {
    int n = shapes_g.size();
    ls = new int[n];
    rs = new int[n];
    boxes = new Box[n];
    for(int i = 0; i < n; ++i) {
        ls[i] = rs[i] = -1;
        boxes[i] = shapes_g[i] -> Bound();
        shapes.push_back(shapes_g[i]);
        aux.push_back(std::make_pair(shapes_g[i], i));
    }
    Build(0, n, root);
    aux.clear();
}

std::pair<int, std::pair<Vec2, Vec3>> AccelKDtree::Inter(Vec3 p, Vec3 d) {
    d.Normalize();
    float dis = 1e30;
    std::pair<Vec2, Vec3> res;
    int id = -1;
    Query(p, d, root, dis, id, res);
    return make_pair(id, res);
}
