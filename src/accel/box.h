/*
bounding box
*/
#include <utility>
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
    Vec3 GetLower();
    Vec3 GetUpper();

    /// @brief add one more point, then change the bound
    void Add(Vec3);

    /// @brief add one more box, then change the bound
    void Add(Box);

    /// @brief if the point is inside the box
    bool Inside(Vec3);

    /// @brief return whether intersected or not, for improved speed
    bool InterIf(Vec3 p, Vec3 d);

    /// @brief assuming d is normalized
    float InterNormed(Vec3 p, Vec3 d);

    /// @brief return length^2
    float InterSquared(Vec3 p, Vec3 d);

    /// @brief return length^2 and assume d is normalized
    /// assuming d is normalized
    float InterSquaredNormed(Vec3 p, Vec3 d);

    /// @brief if not intersected, return <false, inf>
    std::pair<bool, float> Inter(Vec3 p, Vec3 d);
};
#endif
