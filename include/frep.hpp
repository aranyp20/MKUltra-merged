#pragma once

#include <iostream>
#include <functional>
#include <FunctionCreator.h>

// template <typename T>
// T fn(const T& x, const T& y, double h)  {return T(8)*(pow(x,4)+pow(y,4)+pow(T(h),4))-T(8)*(pow(x,2)+pow(y,2)+pow(T(h),2)) +T(3);}
/*
inline interval fn_inter(const interval &i1, const interval &i2, double h)
{
    // return FunctionCreator::Create_interval(i1, i2, h);
    return interval(8) * (pow(i1, 4) + pow(i2, 4) + pow(interval(h), 4)) - interval(8) * (pow(i1, 2) + pow(i2, 2) + pow(interval(h), 2)) + interval(3);
}
inline double fn_sima(double x, double y, double h)
{

    // return FunctionCreator::Create(vec3(x, y, h));
    return 8 * (pow(x, 4) + pow(y, 4) + pow(h, 4)) - 8 * (pow(x, 2) + pow(y, 2) + pow(h, 2)) + 3;

    double val = 8 * (pow(x, 4) + pow(y, 4) + pow(h, 4)) - 8 * (pow(x, 2) + pow(y, 2) + pow(h, 2)) + 3;
    val /= fn_grad(vec3(x, y, h)).length();
    return val + 0.1;
}
*/

inline vec3 fn_grad_changethis(const vec3 &p)
{

    // return FunctionCreator::Create_grad(p);
    return vec3(32 * pow(p.x, 3) - 16 * p.x, 32 * pow(p.y, 3) - 16 * p.y, 32 * pow(p.z, 3) - 16 * p.z);
}

class frep_object
{

public:
    // caller functions: the implementation usually just calls the local template function with its overload parameter types. (template functions can't be virtuals.)
    virtual double fn(const vec3 &) const = 0;
    virtual interval fn(const interval &X, const interval &Y, double h) const = 0;
    virtual vec3 grad(const vec3 &) const = 0;

    bool inside(const vec3 &p) const
    {
        return fn(p) < 0;
    }
};

class chmutov : public frep_object
{
    template <typename T>
    T common_fn(const T &x, const T &y, double h) const
    {
        return T(8) * (pow(x, 4) + pow(y, 4) + pow(T(h), 4)) - T(8) * (pow(x, 2) + pow(y, 2) + pow(T(h), 2)) + T(3);
    }

public:
    double fn(const vec3 &p) const override
    {
        return common_fn(p.x, p.y, p.z);
    }
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    vec3 grad(const vec3 &p) const override
    {
        vec3 res = fn_grad_changethis(p);
        return res;
    }
};