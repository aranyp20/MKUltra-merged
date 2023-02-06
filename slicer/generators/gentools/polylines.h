#pragma once
#include <data_table.hpp>
#include <mymath.h>

struct polylines : public data_table<vec3>
{

    void add_together(const polylines &);
};