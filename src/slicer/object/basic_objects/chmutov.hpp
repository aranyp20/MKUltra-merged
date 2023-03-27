#pragma once

#include "frep.hpp"

class chmutov : public frep_object
{
#if 0
    template <typename T>
    T common_fn_base(const T &x, const T &y, double h) const
    {
        return T(8) * (pow(x, 4) + pow(y, 4) + pow(T(h), 4)) - T(8) * (pow(x, 2) + pow(y, 2) + pow(T(h), 2)) + T(3);
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

#if 1
    template <typename T, typename H>
    T common_fn(const T &x, const T &y, const H &h) const
    {
        return T(8) * (pow(x, 4) + pow(y, 4) + T(pow(h, 4))) - T(8) * (pow(x, 2) + pow(y, 2) + T(pow(h, 2))) + T(3);
    }

#endif

public:
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return common_fn(X, Y, h);
    }
    
    dnum fn(const dnum &X, const dnum& Y, const dnum& h) const override
    {
        return common_fn(X,Y,h);
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