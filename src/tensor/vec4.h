/*
vector in 4d
*/
#ifndef VEC4_H
#define VEC4_H
class Vec4 {
public:
    float x, y, z, w;
    Vec4();
    ~Vec4();
    Vec4(float, float, float, float);
    Vec4 operator+(const Vec4&b);
    Vec4 operator-(const Vec4&b);
    Vec4 operator*(const float d);
    Vec4 operator/(const float d);
    void operator+=(const Vec4&b);
    void operator-=(const Vec4&b);
    void operator*=(const float d);
    void operator/=(const float d);

    /// @brief return the element-wise product: (a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w)
    Vec4 operator*(const Vec4&b);

    /// @brief return x + y + z + w;
    float Sum();

    /// @brief return the dot product: <a, b>
    float Dot(const Vec4&b);

    /// @brief return the norm of this vector, errors will occur if the vector is 0
    Vec4 Norm();

    /// @brief normalize this vector, errors will occur if the vector is 0
    void Normalize();

    /// @brief return the length of the vector, errors will occur if the vector is 0
    float Len();

    /// @brief return the angle in radians between a and b, safely
    float Angle(const Vec4&b);

    /// @brief return the squared length of the vector
    float Square();
};

#endif