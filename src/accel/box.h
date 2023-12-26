/*
bounding box
*/
#include "../tensor/vec3.h"
#ifndef BOX_H
#define BOX_H
class Box {
private:
    /// point with smaller values
    Vec3 lower;
    
    /// point with bigger values
    Vec3 upper;
public:
    Box();

    /// @brief initialization with two points
    Box(Vec3, Vec3);
    ~Box();

    /// @brief add one more point, then change the bound
    void Add(Vec3);
};
#endif