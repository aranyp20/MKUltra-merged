#pragma once

#include <iostream>
#include <functional>
#include <FunctionCreator.h>


#include "settings.h"

struct bounding_box
{
    std::pair<vec3, double> floor;
    double height;

    bounding_box(const vec3 &_corner, double _width, double _height) : floor(std::pair<vec3, double>{_corner, _width}), height(_height) {}

    inline vec3 get_center() const
    {
        return vec3(floor.first.x + (floor.second / 2), floor.first.y + (floor.second / 2), floor.first.z + (height / 2));
    }

    inline double get_size() const { return floor.second; }

    inline double get_height() const { return height; }
};

inline vec3 fn_grad_changethis(const vec3 &p)
{
    return vec3(32 * pow(p.x, 3) - 16 * p.x, 32 * pow(p.y, 3) - 16 * p.y, 32 * pow(p.z, 3) - 16 * p.z);
}

class frep_object
{
protected:
    template <typename T>
    T intersection(const T &v1, const T &v2) const
    {
        return v1 + v2 - sqrt(v1 * v1 + v2 * v2);
    }
public:
    // caller functions: the implementation usually just calls the local template function with its overload parameter types. (template functions can't be virtuals.)
    virtual double fn(const vec3 &) const = 0;
    virtual interval fn(const interval &X, const interval &Y, double h) const = 0;
    virtual vec3 grad(const vec3 &) const = 0;
    virtual void set_prefered_settings() {}
    virtual bounding_box get_prefered_box() const { return bounding_box(vec3(-1.1, -1.1, -1.1), 2.2, 2.2); }

    bool inside(const vec3 &p) const
    {
        return fn(p) < 0;
    }
};

class chmutov : public frep_object
{
#if 1
    template <typename T>
    T common_fn_base(const T &x, const T &y, double h) const
    {
        return T(8) * (pow(x, 4) + pow(y, 4) + pow(T(h), 4)) - T(8) * (pow(x, 2) + pow(y, 2) + pow(T(h), 2)) + T(3);
    }

    template <typename T>
    T intersection(const T &v1, const T &v2) const
    {


        return T(0.5)*(v1 + v2 + sqrt(std::max(T(0), v1 * v1 + v2 * v2- T(2)*v1*v2)));
    }

    template <typename T>
    T common_fn(const T &x, const T &y, double h) const
    {
        T normal_inverted = common_fn_base(x, y, h) * T(-1);
        T scaled_up = common_fn_base(x, y, h) - T(0.8);

        return intersection(normal_inverted, scaled_up);

        return scaled_up;
    }
#endif

#if 0
    template <typename T>
    T common_fn(const T &x, const T &y, double h) const
    {
        return T(8) * (pow(x, 4) + pow(y, 4) + pow(T(h), 4)) - T(8) * (pow(x, 2) + pow(y, 2) + pow(T(h), 2)) + T(3);
    }

#endif

public:
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    vec3 grad(const vec3 &p) const override
    {
        // std::cout << "Out of order function!" << std::endl;

        vec3 res = fn_grad_changethis(p);
        return res;
    }
    double fn(const vec3 &p) const override
    {
        return common_fn(p.x, p.y, p.z);
    }

    void set_prefered_settings() override
    {
        settings::inner_shell_distance = 0.05;
        settings::infill_space_between = 0.1;
    }
};

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
    vec3 grad(const vec3 &p) const override
    {
        return fc.Create_grad(p);
    }
};

class gyroid : public frep_object
{
    template <typename T>
    T common_fn_base(const T &x, const T &y, double h) const
    {
        return cos(x) * sin(y) + cos(y) * sin(T(h)) + cos(T(h)) * sin(x);
    }


    template <typename T>
    T common_fn(const T &x, const T &y, double h) const
    {
        T normal = common_fn_base(x, y, h);
        T inverted_scaled_up = (normal - T(1)) * T(-0.1);

        return intersection(normal, inverted_scaled_up);
    }

protected:
    bounding_box get_prefered_box() const override { return bounding_box(vec3(0, 0, -11), 22, 22); }

public:
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    double fn(const vec3 &p) const override
    {
        return common_fn(p.x, p.y, p.z);
    }

    vec3 grad_base(const vec3 &p) const
    {
        return vec3(cos(p.z) * cos(p.x) - sin(p.y) * sin(p.x), cos(p.x) * cos(p.y) - sin(p.z) * sin(p.y), cos(p.y) * cos(p.z) - sin(p.x) * sin(p.z));
    }

    vec3 grad(const vec3 &p) const
    {
        std::cout << "Out of order function!" << std::endl;
        return vec3();
    }

    void set_prefered_settings() override
    {
        settings::inner_shell_distance = 0.3;
        settings::infill_space_between = 1.2;
    }
};