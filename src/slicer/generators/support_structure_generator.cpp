#include "support_structure_generator.h"
#include "slicer_module.h"

polylines support_structure_generator::create_colors_from_wieghts(const weighted_polylines &wp) const
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

void support_structure_generator::colorize_points_by_neccessity(sliced_object &obj) const
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

std::vector<support_column_template> support_structure_generator::find_column_spaces_onz(const vec2 &p) const
{
    std::vector<support_column_template> result;

    const double step_distance = 0.01;

    double pz = -1;
    double last_leave = pz;
    double last_enter = pz;
    bool last_was_in = surface->inside(vec3(p.x, p.y, pz));

    for (pz = step_distance - 1; pz <= 1; pz += step_distance)
    {
        bool this_is_in = surface->inside(vec3(p.x, p.y, pz));

        if (this_is_in && !last_was_in)
        {
            result.push_back({vec3(p.x, p.y, last_leave), vec3(p.x, p.y, pz)});
            last_enter = pz;
        }
        else if (!this_is_in && last_was_in)
        {
            last_leave = pz;
        }

        last_was_in = this_is_in;
    }

    return result;
}

std::vector<support_column_template> support_structure_generator::find_column_spaces(const double distance_between) const
{
    std::vector<support_column_template> result;

    for (double x = -1 + distance_between / 2; x < 1; x += distance_between)
    {
        for (double y = -1 + distance_between / 2; y < 1; y += distance_between)
        {
            std::vector<support_column_template> on_this_point = find_column_spaces_onz(vec2(x, y));
            result.insert(result.end(), on_this_point.begin(), on_this_point.end());
        }
    }

    return result;
}

support_structure_generator::support_structure_generator(std::shared_ptr<frep_object> _surface) : surface(_surface)
{
}

std::vector<support> support_structure_generator::generate_from_templates(const frep_object &obj, const std::vector<support_column_template> &wheres) const
{
    std::vector<support> result;

    for (const auto &a : wheres)
    {
        result.push_back(support(*surface, a.ground_point, a.hold_point));
    }

    return result;
}

sliced_object support_structure_generator::generate_to(const sliced_object &obj, const double distance_between, unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance, std::function<void(int)> cb) const
{
    sliced_object result = obj;

    std::vector<support> slicable_columns = generate_from_templates(*surface, find_column_spaces(distance_between));

    for (const auto &a : slicable_columns)
    {
        slicer t_slicer(std::make_shared<support>(a));
        result = sliced_object(result, t_slicer.create_slices(level_count, inner_shell_count, inner_shell_distance, cb, true));
    }

    return result;
}
