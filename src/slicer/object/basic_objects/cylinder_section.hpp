#pragma once

#include "frep.hpp"

class cylinder_section : public frep_object
{
    vec3 fix1, fix2;
    double radius = 0.1;

    template <typename T, typename H, typename ARRAY = vec3_t<T>>
    T common_fn(const T &x, const T &y, const H &h) const
    {
        ARRAY p(x, y, T(h));
        ARRAY p1(T(fix1.x), T(fix1.y), T(fix1.z));
        ARRAY p2(T(fix2.x), T(fix2.y), T(fix2.z));

        ARRAY d = (p2 - p1) / (p2 - p1).length();

        return std::max(std::max(p1.z - p.z, p.z - p2.z), ((p1 - p - d * (dot((p1 - p), d))).length() - T(radius)));
    }

public:
    cylinder_section(const vec3 &_fix1, const vec3 &_fix2) : fix1(_fix1), fix2(_fix2) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    double fn(const vec3 &p) const override
    {
        return common_fn<double, double, vec3>(p.x, p.y, p.z);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return common_fn(X, Y, h);
    }
};