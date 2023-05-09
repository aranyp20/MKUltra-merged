#pragma once

#include <memory>
#include "printable_object.h"
#include "frep.hpp"
#include "support_column_template.h"

class support_sctructure_generator
{

    std::shared_ptr<frep_object> surface;

    polylines create_colors_from_wieghts(const weighted_polylines &) const;

    std::vector<support_column_template> find_column_spaces_onz(const vec2 &p) const;
    std::vector<support_column_template> find_column_spaces(const double distance_between) const;

public:
    support_sctructure_generator(std::shared_ptr<frep_object>);

    void generate_to(sliced_object &) const;
};