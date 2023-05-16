#pragma once

#include <memory>
#include "printable_object.h"
#include "frep.hpp"
#include "support.hpp"
#include "support_column_template.h"

class support_structure_generator
{

    std::shared_ptr<frep_object> surface;

    polylines create_colors_from_wieghts(const weighted_polylines &) const;
    void colorize_points_by_neccessity(sliced_object &) const;

    std::vector<support_column_template> find_column_spaces_onz(const vec2 &p) const;
    std::vector<support_column_template> find_column_spaces(const double distance_between) const;

public:
    support_structure_generator(std::shared_ptr<frep_object>);

    support generate_to(const frep_object &) const;
};