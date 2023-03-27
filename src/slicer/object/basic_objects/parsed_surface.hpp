#pragma once


#include "frep.hpp"

class parsed_surface : public frep_object
{

public:
    parsed_surface(const std::string &expr)
    {
    }

    template <typename T, typename H>
    T common_fn(const T &x, const T &y, const H &h) const
    {
        // TODO...
        return T();
    }

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