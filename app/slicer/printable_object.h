#pragma once

#include <array>
#include <float.h>
#include "polylines.h"

struct sliced_obj
{

    std::vector<polylines> data;
    std::array<double, 2> x_boundaries = std::array<double, 2>{DBL_MAX, DBL_MIN};
    std::array<double, 2> y_boundaries = std::array<double, 2>{DBL_MAX, DBL_MIN};
    std::array<double, 2> h_boundaries = std::array<double, 2>{DBL_MAX, DBL_MIN};

    unsigned int slice_count;

    void check_bounds(const vec3 &);
    sliced_obj(const std::vector<polylines> &);
};