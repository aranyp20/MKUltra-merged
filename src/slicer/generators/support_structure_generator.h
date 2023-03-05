#pragma once

#include "printable_object.h"
#include "frep.hpp"

class support_sctructure_generator
{

    frep_object *surface;

    polylines create_colors_from_wieghts(const weighted_polylines &) const;

public:
    support_sctructure_generator(frep_object *);

    void generate_to(sliced_object &) const;
};