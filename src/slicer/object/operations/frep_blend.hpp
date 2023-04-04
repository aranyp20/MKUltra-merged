#pragma once

#include "frep.hpp"

class frep_blend : public frep_object
{
    const frep_object &parent1;
    double scale_val1;
    const frep_object &parent2;
    double scale_val2;

    template <typename T>
    T blend(const T &v1, double r1, const T &v2, double r2) const
    {
        return T(1) - std::max(T(0), T(1) - (v1 / T(r1))) - std::max(T(0), T(1) - (v2 / T(r2)));
    }

public:
    frep_blend(const frep_object &_parent1, double _scale_val1, const frep_object &_parent2, double _scale_val2) : parent1(_parent1), scale_val1(_scale_val1), parent2(_parent2), scale_val2(_scale_val2) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return blend(parent1.fn(X, Y, h), scale_val1, parent2.fn(X, Y, h), scale_val2);
    }
    double fn(const vec3 &p) const override
    {
        return blend(parent1.fn(p), scale_val1, parent2.fn(p), scale_val2);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return blend(parent1.fn(X, Y, h), scale_val1, parent2.fn(X, Y, h), scale_val2);
    }
};