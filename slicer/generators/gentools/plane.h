#pragma once

#include "mymath.h"
#include "section.h"

class plane
{

    vec2 corner;
    double size; // both width and height (expected, don't change)

public:
    plane(const vec2 &, double);
    plane(const std::pair<vec2, double> &);

    vec2 get_center() const;
    double get_diagonal_size() const;
    std::vector<section> get_sides() const;
    double get_size() const;
};