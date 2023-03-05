#pragma once
#include <data_table.hpp>
#include <mymath.h>

struct polylines : public data_table<vec3>
{

    void add_together(const polylines &);
    void eat(const std::vector<vec3> &);
};

struct weighted_polylines : public data_table<std::pair<vec3, double>>
{

    weighted_polylines(const polylines &);
};