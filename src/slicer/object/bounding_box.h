#pragma once

#include <iostream>
#include "mymath.h"

struct bounding_box
{
    std::pair<vec3, double> floor;
    double height;

    bounding_box(const vec3 &_corner, double _width, double _height);

    vec3 get_center() const;

    double get_size() const;

    double get_height() const;
};
