#pragma once

#include <muParser.h>

#include "frep.hpp"

class parsed_surface : public frep_object
{
    mu::Parser parser;

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
    vec3 grad(const vec3 &p) const override
    {

        return vec3(common_fn(dnum(p.x, 1), dnum(p.y, 0), dnum(p.z, 0)).der_val, common_fn(dnum(p.x, 0), dnum(p.y, 1), dnum(p.z, 0)).der_val, common_fn(dnum(p.x, 0), dnum(p.y, 0), dnum(p.z, 1)).der_val);
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