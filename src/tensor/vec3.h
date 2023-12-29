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
    Vec3 Cross(const Vec3&b);

    /// @brief return the element-wise product: (a.x * b.x, a.y * b.y, a.z * b.z)
    Vec3 operator*(const Vec3&b);

    /// @brief return x + y + z;
    float Sum();

    /// @brief return the dot product: <a, b>
    float Dot(const Vec3&b);

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

    /// @brief return (|x|,|y|,|z|)
    Vec3 Abs();

    /// @brief return max(x,y,z)
    float Max();

    /// @brief return min(x,y,z)
    float Min();
};

/// @brief return a x b if a and b is not on a line,
/// otherwise return c that <a,c> = 0
Vec3 CrossSafe(Vec3 a, Vec3 b);
#endif
