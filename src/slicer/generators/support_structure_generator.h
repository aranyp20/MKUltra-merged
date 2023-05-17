#pragma once

#include <memory>
#include "printable_object.h"
#include "frep.hpp"
#include "support.hpp"
#include "support_column_template.h"

class support_structure_generator
{


    polylines create_colors_from_wieghts(const weighted_polylines &) const;
    void colorize_points_by_neccessity(sliced_object &) const;

    std::vector<support_column_template> find_column_spaces_onz(const vec2 &p) const;
    std::vector<support_column_template> find_column_spaces(const double distance_between) const;

    std::vector<std::shared_ptr<support>> generate_from_templates(const std::vector<support_column_template> &wheres) const;

public:
    std::shared_ptr<frep_object> surface;
    support_structure_generator(std::shared_ptr<frep_object>);

    sliced_object generate_to(const sliced_object &, const double distance_between, unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance, std::function<void(int)>) const;
};