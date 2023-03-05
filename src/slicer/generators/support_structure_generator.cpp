#include "support_structure_generator.h"

polylines support_sctructure_generator::create_colors_from_wieghts(const weighted_polylines &wp) const
{
    polylines result;

    auto colored = [&](std::pair<vec3, double> d) -> vec3
    {
        return vec3(0, std::max(1.0, std::min(0.0, 1 * d.second)), 0);
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

support_sctructure_generator::support_sctructure_generator(frep_object *_surface) : surface(_surface)
{
}

void support_sctructure_generator::generate_to(sliced_object &obj) const
{
    weighted_polylines weighted(obj.get_poly_level(5, sliced_object::layer_data::part_type::OUTER));

    obj.set_level_color(create_colors_from_wieghts(weighted), 5, sliced_object::layer_data::part_type::OUTER);
}
