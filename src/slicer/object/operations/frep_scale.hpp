#pragma once

#include "frep.hpp"

// common_fn kell
class frep_scale : public frep_object
{
    const frep_object &parent;
    double scale_val;

public:
    frep_scale(const frep_object &_parent, double _scale_val) : parent(_parent), scale_val(_scale_val) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return parent.fn(X, Y, h) - interval(scale_val);
    }
    double fn(const vec3 &p) const override
    {
        return parent.fn(p) - scale_val;
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return parent.fn(X, Y, h) - dnum(scale_val, 0);
    }
};