#pragma once

#include "frep.hpp"
#include "frep_scale.hpp"
#include "frep_invert.hpp"
#include "frep_intersect.hpp"

class fdm_obj : public frep_object
{
    std::shared_ptr<frep_object> original;

    std::shared_ptr<frep_object> infill;

    frep_scale offset_obj;

    frep_invert offset_inverted;

    frep_intersect wall;

    frep_intersect shaped_infill;

    frep_onion ready;

public:
    fdm_obj(const std::shared_ptr<frep_object> _original, const std::shared_ptr<frep_object> _infill, float offset) : original(_original), infill(_infill), offset_obj(frep_scale(*original, -0.2)), offset_inverted(frep_invert(offset_obj)), wall(frep_intersect(*original, offset_inverted)), shaped_infill(frep_intersect(offset_obj, *infill)), ready(frep_onion(shaped_infill, wall)) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return ready.fn(X, Y, h);
    }
    double fn(const vec3 &p) const override
    {
        return ready.fn(p);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return ready.fn(X, Y, h);
    }
};