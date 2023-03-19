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

support_sctructure_generator::support_sctructure_generator(frep_object *_surface) : surface(_surface)
{
}

void support_sctructure_generator::generate_to(sliced_object &obj) const
{
    auto weighed = [&](const vec3 &p, frep_object *surf) -> double
    {
        return dot(vec3(0.0, 0.0, -1.0), normalize(surf->grad(p)));
    };

    for (int level = 0; level < obj.get_slice_count(); level++)
    {
        weighted_polylines weighted(obj.get_poly_level(level, sliced_object::layer_data::part_type::OUTER), weighed, surface);

        obj.set_level_color(create_colors_from_wieghts(weighted), level, sliced_object::layer_data::part_type::OUTER);
    }
}
