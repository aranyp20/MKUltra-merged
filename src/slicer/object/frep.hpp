#pragma once

#include <iostream>
#include <functional>

#include "interval.h"
#include "settings.h"
#include "bounding_box.h"
#include "dual_num.h"

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
        return T(0.5) * (v1 + v2 + sqrt(std::max(T(0), v1 * v1 + v2 * v2 - T(2) * v1 * v2)));
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
