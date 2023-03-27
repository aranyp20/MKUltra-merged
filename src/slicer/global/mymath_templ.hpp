#pragma once

#include <iostream>

#include <cmath>


template <typename T>
struct vec3_t
{

    T x, y, z;

    vec3_t(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

    vec3_t operator*(T num) const { return vec3_t(x * num, y * num, z * num); }
    vec3_t operator/(T num) const { return vec3_t(x / num, y / num, z / num); }
    vec3_t operator+(const vec3_t &v) const { return vec3_t(x + v.x, y + v.y, z + v.z); }
    vec3_t operator-(const vec3_t &v) const { return vec3_t(x - v.x, y - v.y, z - v.z); }
    vec3_t operator*(const vec3_t &v) const { return vec3_t(x * v.x, y * v.y, z * v.z); }

    virtual inline T length() const { return sqrt(x * x + y * y + z * z); }

    inline void normalize()
    {

        vec3_t newVec = vec3_t(x, y, z) * (1 / length());
        x = newVec.x;
        y = newVec.y;
        z = newVec.z;
    }
};


template <typename T>
inline std::ostream &operator<<(std::ostream &os, const vec3_t<T> &v)
{
    os << v.x << " " << v.y << " " << v.z << std::endl;
    return os;
}

template <typename T>
inline vec3_t<T> normalize(const vec3_t<T> &v) { return v / v.length(); }

template <typename T>
inline T dot(const vec3_t<T> &v1, const vec3_t<T> &v2) { return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

template <typename T>
inline vec3_t<T> cross(const vec3_t<T> &v1, const vec3_t<T> &v2)
{
    return vec3_t(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}