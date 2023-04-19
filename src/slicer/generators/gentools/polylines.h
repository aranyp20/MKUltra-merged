#pragma once
#include <memory>
#include <functional>
#include "data_table.hpp"
#include "mymath.h"
#include "frep.hpp"

struct polylines : public data_table<vec3>
{

    void add_together(const polylines &);
    void eat(const std::vector<vec3> &);
};

struct weighted_polylines : public data_table<std::pair<vec3, double>>
{

    weighted_polylines(const polylines &, std::function<double(const vec3 &, std::shared_ptr<frep_object>)> weigher_function, std::shared_ptr<frep_object>);
};