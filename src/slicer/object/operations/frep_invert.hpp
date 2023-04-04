#pragma once

#include "frep.hpp"

class frep_invert : public frep_object
{
    const frep_object &parent;

    template <typename T>
    T invert(const T &v1) const
    {
        return T(-1) * v1;
    }

public:
    frep_invert(const frep_object &_parent) : parent(_parent) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return invert(parent.fn(X, Y, h));
    }
    double fn(const vec3 &p) const override
    {
        return invert(parent.fn(p));
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return invert(parent.fn(X, Y, h));
    }
};