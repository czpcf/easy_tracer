/*
represents f(x) = Ax + b from R^3 to R^3,
where A is orthogonal,
x, y, z axis must be scaled with the same value
*/
#include "vec3.h"
#ifndef MAT3_H
#define MAT3_H
class Mat3 {
private:
    /// entris of matrix A
    /// very ugly representation... for the sake of speed
    float a[9];
    /// the vector b
    float b[3];
public:
    /// @brief the representation of the transform f(x) = Ax + b,
    /// where A is a 3*3 matrix and b is a 3d vector
    Mat3();
    ~Mat3();
    
    /// @brief return the transform which represents a shift: 0 -> p
    Mat3(Vec3 p);
    
    /// @brief return the transform which represents a counter-clockwise
    /// rotation around the vector n with theta radians
    Mat3(Vec3 n, float theta);

    /// @brief the representation of the transform f(x) = Ax + b,
    /// where A is a 3*3 matrix and b is a 3d vector
    Mat3(float array_a[9], float array_b[3]);

    /// @brief the representation of the transform f(x) = Ax + b,
    /// where A is a 3*3 matrix and b is a 3d vector
    Mat3(float matrix_a[3][3], float array_b[3]);
    
    /// @brief return f(g(x))
    Mat3 operator*(const Mat3&g);

    /// @brief return f(x) = I * x + 0
    Mat3 Id();

    /// @brief return f(x) = I * x + a
    Mat3 Trans(const Vec3&a);

    /// @brief return f(a)
    Vec3 Map(const Vec3&a);

    /// @brief return Ax
    Vec3 MapA(const Vec3&x);

    /// @brief return det A
    float Det();
};

/// @brief transformation that (0,0,1) -> z + p, (1,0,0) -> x + p
Mat3 TransZXP(Vec3 z, Vec3 x, Vec3 p);
#endif