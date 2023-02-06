#pragma once

#include <mymath.h>
#include <vector>

struct section
{

    vec2 p1, p2;

    section(const vec2 &_p1, const vec2 &_p2);

    double length() const;
    void swap_sides();
};

std::vector<vec3> serialize(const std::vector<section> &v, double h);