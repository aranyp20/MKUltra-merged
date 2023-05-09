#include "support_structure_generator.h"

polylines support_sctructure_generator::create_colors_from_wieghts(const weighted_polylines &wp) const
{
    polylines result;

    auto colored = [&](std::pair<vec3, double> d) -> vec3
    {
        return vec3(1.0, 0, std::min(1.0, std::max(0.0, d.second)));
    };

    for (int i = 0; i < wp.data.size(); i++)
    {
        result.data.push_back(std::vector<vec3>());
        for (int j = 0; j < wp.data[i].size(); j++)
        {
            result.data[i].push_back(colored(wp.data[i][j]));
        }
    }

    return result;
}

std::vector<support_column_template> support_sctructure_generator::find_column_spaces_onz(const vec2 &p) const
{
    return std::vector<support_column_template>();
}

std::vector<support_column_template> support_sctructure_generator::find_column_spaces(const double distance_between) const
{
    std::vector<support_column_template> result;

    for (double x = distance_between / 2; x < 2; x += distance_between)
    {
        for (double y = distance_between / 2; y < 2; y += distance_between)
        {
            std::vector<support_column_template> on_this_point = find_column_spaces_onz(vec2(x, y));
            result.insert(result.end(), on_this_point.begin(), on_this_point.end());
        }
    }

    return result;
}

support_sctructure_generator::support_sctructure_generator(std::shared_ptr<frep_object> _surface) : surface(_surface)
{
}

void support_sctructure_generator::generate_to(sliced_object &obj) const
{
    auto weighed = [&](const vec3 &p, std::shared_ptr<frep_object> surf) -> double
    {
        return dot(vec3(0.0, 0.0, -1.0), normalize(surf->grad(p)));
    };

    for (int level = 0; level < obj.get_slice_count(); level++)
    {
        weighted_polylines weighted(obj.get_poly_level(level, sliced_object::layer_data::part_type::OUTER), weighed, surface);

        obj.set_level_color(create_colors_from_wieghts(weighted), level, sliced_object::layer_data::part_type::OUTER);
    }
}
