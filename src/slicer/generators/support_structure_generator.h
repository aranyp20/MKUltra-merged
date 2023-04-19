#pragma once

#include <memory>
#include "printable_object.h"
#include "frep.hpp"

class support_sctructure_generator
{

    std::shared_ptr<frep_object> surface;

    polylines create_colors_from_wieghts(const weighted_polylines &) const;

public:
    support_sctructure_generator(std::shared_ptr<frep_object>);

    void generate_to(sliced_object &) const;
};