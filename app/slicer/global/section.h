#pragma once

#include <mymath.h>
#include <vector>
#include "basics.hpp"

struct section
{

    vec2 p1, p2;

    section(const vec2 &_p1, const vec2 &_p2);
    section() {}

    double length() const;
    void swap_sides();

    opt_return<vec2> intersect(const section &) const;
};

std::vector<vec3> serialize(const std::vector<section> &v, double h);