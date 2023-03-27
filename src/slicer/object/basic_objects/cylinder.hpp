#pragma once

#include "frep.hpp"

class cylinder : public frep_object
{
    vec3 dir = vec3(0, 0, 1);
    vec3 fix_point = vec3();
    double radius = 0.8;

    template <typename T, typename H, typename ARRAY = vec3_t<T>>
    T common_fn(const T &x, const T &y, const H &h) const
    {
        ARRAY p(x, y, T(h));
        ARRAY q(T(fix_point.x), T(fix_point.y), T(fix_point.z));
        ARRAY d(T(dir.x), T(dir.y), T(dir.z));

        return (q - p - d * (dot((q - p), d))).length() - T(radius);
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
    dnum fn(const dnum &X, const dnum& Y, const dnum& h) const override
    {
        return common_fn(X,Y,h);
    }
};