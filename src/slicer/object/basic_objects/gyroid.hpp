#pragma once

#include "frep.hpp"

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

    vec3 grad_base(const vec3 &p) const
    {
        return vec3(cos(p.z) * cos(p.x) - sin(p.y) * sin(p.x), cos(p.x) * cos(p.y) - sin(p.z) * sin(p.y), cos(p.y) * cos(p.z) - sin(p.x) * sin(p.z));
    }

    vec3 grad(const vec3 &p) const
    {
        // std::cout << "Out of order function!" << std::endl;
        return grad_base(p);
    }

    void set_prefered_settings() override
    {
        settings::inner_shell_distance = 0.3;
        settings::infill_space_between = 1.2;
    }
};