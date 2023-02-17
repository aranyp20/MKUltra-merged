#pragma once

#include <array>
#include <float.h>
#include "polylines.h"
#include "plane.h"

struct sliced_obj
{

    std::vector<polylines> data;
    plane bounding_plane;
    unsigned int slice_count;

    std::vector<std::vector<float>> filled_data;

    void normalize_for_gl(vec3 &, const plane &) const;

    sliced_obj(const std::vector<polylines> &, const plane &);
};