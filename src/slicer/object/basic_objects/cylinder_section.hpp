#pragma once

#include "frep.hpp"

class cylinder_section : public frep_object
{
    vec3 fix1 = vec3(0, 0, -0.9);
    vec3 fix2 = vec3(0, 0, 0);
    double radius = 0.2;

    template <typename T, typename H, typename ARRAY = vec3_t<T>>
    T common_fn(const T &x, const T &y, const H &h) const
    {
        ARRAY p(x, y, T(h));
        ARRAY p1(T(fix1.x), T(fix1.y), T(fix1.z));
        ARRAY p2(T(fix2.x), T(fix2.y), T(fix2.z));

        ARRAY d = (p2 - p1) / (p2 - p1).length();

        if (p2.z < p.z || p.z < p1.z)
        {
            return T(1);
        }
        return (p1 - p - d * (dot((p1 - p), d))).length() - T(radius);
    }

public:
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