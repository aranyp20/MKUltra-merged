#pragma once

#include "frep.hpp"
#include "frep_scale.hpp"
#include "cylinder.hpp"

class sphere : public frep_object
{

    vec3 center;
    double radius = 1.0;

    template <typename T, typename H, typename ARRAY = vec3_t<T>>
    T common_fn(const T &x, const T &y, const H &h) const
    {
        ARRAY p(x, y, T(h));
        ARRAY q(T(center.x), T(center.y), T(center.z));
        T s1 = (p - q).length() - T(radius);

        return s1;
    }

public:
    sphere(const vec3 &_center = vec3(0.0, 0.0, 0.9)) : center(_center) {}

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
