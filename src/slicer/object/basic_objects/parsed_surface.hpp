#pragma once

#include "frep.hpp"

class parsed_surface : public frep_object
{
    template <typename T>
    T put_in_four(const T &v, double period)
    {
        T res = v;
        double sign = -1.0;
        if (res < T(0))
            sign = 1.0;

        // ha lebegopont miatt kicsuszik akkor sokaig megy
        while (res < -T(period) / 2 || res > T(period) / 2)
        {
            res = res + T(sign) * T(period);
        }

        return res;
    }

    template <typename T, typename H>
    T common_fn(const T &x, const T &y, const H &h) const
    {
        // TODO...
        return T();
    }

public:
    parsed_surface(const std::string &expr)
    {
    }

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return common_fn(X, Y, h);
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