#pragma once

#include "frep.hpp"
#include "frep_scale.hpp"
#include "frep_invert.hpp"
#include "frep_intersect.hpp"

class fdm_obj : public frep_object
{
    std::shared_ptr<frep_object> original;

    frep_scale offset_obj;

    frep_invert offset_inverted;

    frep_intersect wall;

public:
    fdm_obj(const std::shared_ptr<frep_object> _original, float offset) : original(_original), offset_obj(frep_scale(*original, -0.2)), offset_inverted(frep_invert(offset_obj)), wall(frep_intersect(*original, offset_inverted)) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return wall.fn(X, Y, h);
    }
    double fn(const vec3 &p) const override
    {
        return wall.fn(p);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return wall.fn(X, Y, h);
    }
};