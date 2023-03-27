#pragma once

#include "frep.hpp"
#include "FunctionCreator.h"

class RBF_surface : public frep_object
{

    FunctionCreator fc;

public:
    double fn(const vec3 &p) const override
    {
        return fc.Create(p);
    }
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return fc.Create_interval(X, Y, h);
    }
    /*
    vec3 grad(const vec3 &p) const override
    {
        return fc.Create_grad(p);
    }
    */
    dnum fn(const dnum &X, const dnum& Y, const dnum& h) const override
    {
        //return common_fn(X,Y,h);
        return dnum(0);
    }
};