#pragma once

#include <iostream>
#include <functional>

#include "interval.h"
#include "settings.h"
#include "bounding_box.h"
#include "dual_num.h"

class frep_object
{
protected:
    template <typename T>
    T invert(const T &v1) const
    {
        return T(-1) * v1;
    }

    template <typename T>
    T intersection(const T &v1, const T &v2) const
    {
        return T(0.5) * (v1 + v2 + sqrt(std::max(T(0), v1 * v1 + v2 * v2 - T(2) * v1 * v2)));
    }

    template <typename T>
    T onion(const T &v1, const T &v2) const
    {
        return T(0.5) * (v1 + v2 - sqrt(std::max(T(0), v1 * v1 + v2 * v2 - T(2) * v1 * v2)));
    }

    template <typename T>
    T scale(const T &v1, double val) const
    {
        return v1 - T(val);
    }

    template <typename T>
    T substract(const T &v1, const T &v2) const
    {
        return intersection(v1, invert(intersection(v1, v2)));
    }

    template <typename T>
    T blend(const T &v1, double r1, const T &v2, double r2) const
    {
        return T(1) - std::max(T(0), T(1) - (v1 / T(r1))) - std::max(T(0), T(1) - (v2 / T(r2)));
    }

public:
    // caller functions: the implementation usually just calls the local template function with its overload parameter types. (template functions can't be virtuals.)
    virtual double fn(const vec3 &) const = 0;
    virtual interval fn(const interval &X, const interval &Y, double h) const = 0;
    virtual dnum fn(const dnum &X, const dnum& Y, const dnum& h) const =0;

    virtual void set_prefered_settings() {}
    virtual bounding_box get_prefered_box() const { return bounding_box(vec3(-1.1, -1.1, -1.1), 2.2, 2.2); }

    vec3 grad(const vec3 &p) const
    {
        return vec3(fn(dnum(p.x, 1), dnum(p.y, 0), dnum(p.z, 0)).der_val, fn(dnum(p.x, 0), dnum(p.y, 1), dnum(p.z, 0)).der_val, fn(dnum(p.x, 0), dnum(p.y, 0), dnum(p.z, 1)).der_val);
    }


    bool inside(const vec3 &p) const
    {
        return fn(p) < 0;
    }
};
