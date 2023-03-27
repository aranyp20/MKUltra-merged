#pragma once

#include "frep.hpp"

class gyroid : public frep_object
{
    template <typename T, typename H>
    T common_fn_base(const T &x, const T &y, const H& h) const
    {
        //return cos(x) * sin(y) + cos(y) * sin(T(h)) + cos(T(h)) * sin(x);
        return T(0);
    }

    template <typename T, typename H>
    T common_fn(const T &x, const T &y, const H& h) const
    {
        T normal_inverted = common_fn_base(x, y, h) * T(-1);
        T scaled_up = common_fn_base(x, y, h) - T(0.8);

        return intersection(normal_inverted, scaled_up);
    }

protected:
    bounding_box get_prefered_box() const override { return bounding_box(vec3(-11, -11, -11), 22, 22); }

public:
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    double fn(const vec3 &p) const override
    {
        return common_fn(p.x, p.y, p.z);
    }

    dnum fn(const dnum &X, const dnum& Y, const dnum& h) const override
    {
        return common_fn(X,Y,h);
    }

    void set_prefered_settings() override
    {
        settings::inner_shell_distance = 0.3;
        settings::infill_space_between = 1.2;
    }
};