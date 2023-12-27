/*
vector in 2d
*/
#ifndef VEC2_H
#define VEC2_H
class Vec2 {
public:
    float x, y;
    Vec2();
    ~Vec2();
    Vec2(float, float);
    Vec2 operator+(const Vec2&b);
    Vec2 operator-(const Vec2&b);
    Vec2 operator*(const float d);
    Vec2 operator/(const float d);
    void operator+=(const Vec2&b);
    void operator-=(const Vec2&b);
    void operator*=(const float d);
    void operator/=(const float d);

    /// @brief return the element-wise product: (a.x * b.x, a.y * b.y)
    Vec2 operator*(const Vec2&b);

    /// @brief return x + y + z;
    float Sum();

    float Dot(const Vec2&b);

    /// @brief return the norm of this vector, errors will occur if the vector is 0
    Vec2 Norm();

    /// @brief normalize this vector, errors will occur if the vector is 0
    void Normalize();

    /// @brief return the length of the vector, errors will occur if the vector is 0
    float Len();

    /// @brief return the angle in radians between a and b, safely
    float Angle(const Vec2&b);

    /// @brief return the squared length of the vector
    float Square();
};

#endif