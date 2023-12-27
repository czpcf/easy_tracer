/*
KDtree accelerator
*/
#ifndef ACCEL_KDTREE_H
#define ACCEL_KDTREE_H
#include "accel.h"
class AccelKDtree: public Accel {
private:
    int n;
    std::vector<Geometry*> shapes;
    std::vector<std::pair<Geometry*, int>> aux;
    int root;
    int* ls;
    int* rs;
    Box* boxes;

    /// @brief build tree
    void Build(int l, int r, int& num);

    /// @brief query the ray
    void Query(const Vec3& p, const Vec3& d, int num, float& dis, int& id, std::pair<Vec2, Vec3>& res);
public:
    AccelKDtree();
    ~AccelKDtree();

    /// @brief initialize the acclerator with shapes
    void Init(std::vector<Geometry*>& shapes);
    
    /// @brief intersection with the line from point p in direction d
    /// if intersected, return <shape id, <local coordinate, normal>>
    /// otherwise <-1, <(0, 0), (0, 0, 0)>>
    std::pair<int, std::pair<Vec2, Vec3>> Inter(Vec3 p, Vec3 d);
};
#endif
