/*
vector in 3d
*/
#ifndef VEC3_H
#define VEC3_H
class Vec3 {
public:
    float x, y, z;
    Vec3();
    ~Vec3();
    Vec3(float, float, float);
    Vec3 operator+(const Vec3&b);
    Vec3 operator-(const Vec3&b);
    Vec3 operator*(const float d);
    Vec3 operator/(const float d);
    void operator+=(const Vec3&b);
    void operator-=(const Vec3&b);
    void operator*=(const float d);
    void operator/=(const float d);

    /// @brief return the cross product: a x b
    Vec3 operator*(const Vec3&b);

    /// @brief return the dot product: <a, b>
    float operator&(const Vec3&b);

    /// @brief return the norm of this vector, errors will occur if the vector is 0
    Vec3 Norm();

    /// @brief normalize this vector, errors will occur if the vector is 0
    void Normalize();

    /// @brief return the length of the vector, errors will occur if the vector is 0
    float Len();

    /// @brief return the angle in radians between a and b, safely
    float Angle(const Vec3&b);

    /// @brief return the squared length of the vector
    float Square();

    /// @brief reflection via b
    Vec3 Reflect(const Vec3&b);

    /// @brief rotate by b with theta radians, counter-clockwise
    Vec3 Rotate(const Vec3&b, float theta);
};
#endif
