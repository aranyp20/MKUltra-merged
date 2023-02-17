#pragma once

#include <iostream>
#include <functional>
#include <FunctionCreator.h>

struct bounding_box
{
    std::pair<vec3, double> floor;
    double height;

    bounding_box(const vec3 &_corner, double _width, double _height) : floor(std::pair<vec3, double>{_corner, _width}), height(_height) {}
};

inline vec3 fn_grad_changethis(const vec3 &p)
{
    return vec3(32 * pow(p.x, 3) - 16 * p.x, 32 * pow(p.y, 3) - 16 * p.y, 32 * pow(p.z, 3) - 16 * p.z);
}

class frep_object
{

public:
    // caller functions: the implementation usually just calls the local template function with its overload parameter types. (template functions can't be virtuals.)
    virtual double fn(const vec3 &) const = 0;
    virtual interval fn(const interval &X, const interval &Y, double h) const = 0;
    virtual vec3 grad(const vec3 &) const = 0;

    virtual bounding_box get_prefered_box() const { return bounding_box(vec3(-1.1, -1.1, -1.1), 2.2, 2.2); }

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
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    vec3 grad(const vec3 &p) const override
    {
        vec3 res = fn_grad_changethis(p);
        return res;
    }
    double fn(const vec3 &p) const override
    {
        return common_fn(p.x, p.y, p.z);
    }
};

class RBF_surface : public frep_object
{

    FunctionCreator fc;

public:
    double fn(const vec3 &p) const override
    {
        return FunctionCreator::Create(p);
    }
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return FunctionCreator::Create_interval(X, Y, h);
    }
    vec3 grad(const vec3 &p) const override
    {
        return FunctionCreator::Create_grad(p);
    }
};

class gyroid : public frep_object
{
    template <typename T>
    T common_fn(const T &x, const T &y, double h) const
    {
        return cos(x) * sin(y) + cos(y) * sin(T(h)) + cos(T(h)) * sin(x);
    }

    bounding_box get_prefered_box() const override { return bounding_box(vec3(-11, -11, -11), 22, 22); }

public:
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    vec3 grad(const vec3 &p) const override
    {
        return vec3(cos(p.z) * cos(p.x) - sin(p.y) * sin(p.x), cos(p.x) * cos(p.y) - sin(p.z) * sin(p.y), cos(p.y) * cos(p.z) - sin(p.x) * sin(p.z));
    }
    double fn(const vec3 &p) const override
    {
        return common_fn(p.x, p.y, p.z);
    }
};