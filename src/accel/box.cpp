#include "box.h"
#include "../const.h"
#include <errno.h>

Box::Box() {
}

Box::~Box() {
}

Vec3 Box::GetLower() {
    return lower;
}

Vec3 Box::GetUpper() {
    return upper;
}

Vec3 MinVec3(const Vec3& a, const Vec3& b) {
    return Vec3(a.x < b.x ? a.x : b.x,
                a.y < b.y ? a.y : b.y,
                a.z < b.z ? a.z : b.z);
}

Vec3 MaxVec3(const Vec3& a, const Vec3& b) {
    return Vec3(a.x > b.x ? a.x : b.x,
                a.y > b.y ? a.y : b.y,
                a.z > b.z ? a.z : b.z);
}

Box::Box(Vec3 p1, Vec3 p2) {
    lower = MinVec3(p1, p2);
    upper = MaxVec3(p1, p2);
}

void Box::Add(Vec3 p) {
    lower = MinVec3(lower, p);
    upper = MaxVec3(upper, p);
}

void Box::Add(Box b) {
    lower = MinVec3(lower, b.lower);
    upper = MaxVec3(upper, b.upper);
}

bool Box::Inside(Vec3 p) {
    return lower.x <= p.x && p.x <= upper.x &&
           lower.y <= p.y && p.y <= upper.y &&
           lower.z <= p.z && p.z <= upper.z;
}

// TODO: be more efficient
bool Box::InterIf(Vec3 p, Vec3 d) {
    auto check = [&](float dx, float dy, float dz, float px, float py, float pz,
        float lower_x, float upper_x, float lower_y, float upper_y, float lower_z, float upper_z) {
        float dis;
        if(dx >= EPS) {
            if(lower_x <= px && px <= upper_x) {
                dis = (upper_x - px) / dx;
            } else {
                dis = (lower_x - px) / dx;
            }
        } else if(dx <= -EPS) {
            if(lower_x <= px && px <= upper_x) {
                dis = (lower_x - px) / dx;
            } else {
                dis = (upper_x - px) / dx;
            }
        } else {
            return false;
        }
        py += dis * dy;
        pz += dis * dz;
        if(!(lower_y <= py && py <= upper_y &&
             lower_z <= pz && pz <= upper_z)) {
            return false;
        }
        return true;
    };
    if(check(d.x, d.y, d.z, p.x, p.y, p.z, lower.x, upper.x, lower.y, upper.y, lower.z, upper.z) ||
       check(d.y, d.x, d.z, p.y, p.x, p.z, lower.y, upper.y, lower.x, upper.x, lower.z, upper.z) ||
       check(d.z, d.x, d.y, p.z, p.x, p.y, lower.z, upper.z, lower.x, upper.x, lower.y, upper.y)) {
        return true;
    }
    return false;
}

// TODO: be more efficient
float Box::InterNormed(Vec3 p, Vec3 d) {
    float dis = 1e38;
    auto check = [&](float dx, float dy, float dz, float px, float py, float pz,
        float lower_x, float upper_x, float lower_y, float upper_y, float lower_z, float upper_z) {
        float ndis;
        if(dx >= EPS) {
            if(lower_x <= px && px <= upper_x) {
                ndis = (upper_x - px) / dx;
            } else {
                ndis = (lower_x - px) / dx;
            }
        } else if(dx <= -EPS) {
            if(lower_x <= px && px <= upper_x) {
                ndis = (lower_x - px) / dx;
            } else {
                ndis = (upper_x - px) / dx;
            }
        } else {
            return;
        }
        py += ndis * dy; // be careful
        pz += ndis * dz;
        if(!(lower_y <= py && py <= upper_y &&
             lower_z <= pz && pz <= upper_z)) {
            return;
        }
        dis = std::min(dis, ndis);
    };
    check(d.x, d.y, d.z, p.x, p.y, p.z, lower.x, upper.x, lower.y, upper.y, lower.z, upper.z);
    check(d.y, d.x, d.z, p.y, p.x, p.z, lower.y, upper.y, lower.x, upper.x, lower.z, upper.z);
    check(d.z, d.x, d.y, p.z, p.x, p.y, lower.z, upper.z, lower.x, upper.x, lower.y, upper.y);
    return dis;
}

// TODO: be more efficient
float Box::InterSquared(Vec3 p, Vec3 d) {
    float dis = 1e38;
    auto check = [&](float dx, float dy, float dz, float px, float py, float pz,
        float lower_x, float upper_x, float lower_y, float upper_y, float lower_z, float upper_z) {
        float ndis;
        if(dx >= EPS) {
            if(lower_x <= px && px <= upper_x) {
                ndis = (upper_x - px) / dx;
            } else {
                ndis = (lower_x - px) / dx;
            }
        } else if(dx <= -EPS) {
            if(lower_x <= px && px <= upper_x) {
                ndis = (lower_x - px) / dx;
            } else {
                ndis = (upper_x - px) / dx;
            }
        } else {
            return;
        }
        py += ndis * dy; // be careful
        pz += ndis * dz;
        if(!(lower_y <= py && py <= upper_y &&
             lower_z <= pz && pz <= upper_z)) {
            return;
        }
        dis = std::min(dis, ndis);
    };
    check(d.x, d.y, d.z, p.x, p.y, p.z, lower.x, upper.x, lower.y, upper.y, lower.z, upper.z);
    check(d.y, d.x, d.z, p.y, p.x, p.z, lower.y, upper.y, lower.x, upper.x, lower.z, upper.z);
    check(d.z, d.x, d.y, p.z, p.x, p.y, lower.z, upper.z, lower.x, upper.x, lower.y, upper.y);
    return dis * dis * d.Square();
}

// TODO: be more efficient
float Box::InterSquaredNormed(Vec3 p, Vec3 d) {
    float dis = 1e38;
    auto check = [&](float dx, float dy, float dz, float px, float py, float pz,
        float lower_x, float upper_x, float lower_y, float upper_y, float lower_z, float upper_z) {
        float ndis;
        if(dx >= EPS) {
            if(lower_x <= px && px <= upper_x) {
                ndis = (upper_x - px) / dx;
            } else {
                ndis = (lower_x - px) / dx;
            }
        } else if(dx <= -EPS) {
            if(lower_x <= px && px <= upper_x) {
                ndis = (lower_x - px) / dx;
            } else {
                ndis = (upper_x - px) / dx;
            }
        } else {
            return;
        }
        py += ndis * dy; // be careful
        pz += ndis * dz;
        if(!(lower_y <= py && py <= upper_y &&
             lower_z <= pz && pz <= upper_z)) {
            return;
        }
        dis = std::min(dis, ndis);
    };
    check(d.x, d.y, d.z, p.x, p.y, p.z, lower.x, upper.x, lower.y, upper.y, lower.z, upper.z);
    check(d.y, d.x, d.z, p.y, p.x, p.z, lower.y, upper.y, lower.x, upper.x, lower.z, upper.z);
    check(d.z, d.x, d.y, p.z, p.x, p.y, lower.z, upper.z, lower.x, upper.x, lower.y, upper.y);
    return dis * dis;
}

// TODO: be more efficient
std::pair<bool, float> Box::Inter(Vec3 p, Vec3 d) {
    float dis1, dis2, dis3;
    auto check = [&](float& dis,
        float dx, float dy, float dz, float px, float py, float pz,
        float lower_x, float upper_x, float lower_y, float upper_y, float lower_z, float upper_z) {

        if(dx >= EPS) {
            if(lower_x <= px && px <= upper_x) {
                dis = (upper_x - px) / dx;
            } else {
                dis = (lower_x - px) / dx;
            }
        } else if(dx <= -EPS) {
            if(lower_x <= px && px <= upper_x) {
                dis = (lower_x - px) / dx;
            } else {
                dis = (upper_x - px) / dx;
            }
        } else {
            dis = 1e38;
            return;
        }
        py += dy * dis;
        pz += dz * dis;
        if(!(lower_y <= py && py <= upper_y &&
             lower_z <= pz && pz <= upper_z)) {
            dis = 1e38;
        }
    };
    check(dis1, d.x, d.y, d.z, p.x, p.y, p.z, lower.x, upper.x, lower.y, upper.y, lower.z, upper.z);
    check(dis2, d.y, d.x, d.z, p.y, p.x, p.z, lower.y, upper.y, lower.x, upper.x, lower.z, upper.z);
    check(dis3, d.z, d.x, d.y, p.z, p.x, p.y, lower.z, upper.z, lower.x, upper.x, lower.y, upper.y);
    float dis = 1e38;
    if(dis1 < dis) {
        dis = dis1;
    }
    if(dis2 < dis) {
        dis = dis2;
    }
    if(dis3 < dis) {
        dis = dis3;
    }
    if(dis > 1e37) {
        return std::make_pair(false, 0.0f);
    }
    return std::make_pair(true, dis);
}
