#include "infill_generator.h"

#include "frep_scale.hpp"

infill_generator::infill_generator(std::shared_ptr<frep_object> _surface) : surface(_surface)
{
}

infill_generator::marked_point::marked_point(const vec3 &s, bool _enters) : vec3(s), enters(_enters) {}

polylines infill_generator::generate(const plane &plane, double h, double angle_between, double space_between, double wall_thickness) const
{
    polylines result;

    double current_angle = 0;
    while (current_angle < M_PI)
    {

        result.add_together(generate_one(plane, h, current_angle, space_between, wall_thickness));

        current_angle += angle_between;
    }

    return result;
}

polylines infill_generator::generate_one(const plane &plane, double h, double angle, double space_between, double wall_thickness) const
{
    polylines result;

    std::vector<section> relevants = generate_relevant_sections_angle(plane, h, angle, space_between);

    for (int i = 0; i < relevants.size(); i++)
    {
        result.add_together(trace_section(relevants[i], plane.get_size() / 100, h, wall_thickness));

        // result.eat(std::vector<vec3>{vec3(relevants[i].p1.x, relevants[i].p1.y, h), vec3(relevants[i].p2.x, relevants[i].p2.y, h)});
    }

    // shrink_polylines(result, wall_thickness);

    return result;
}

void infill_generator::shrink_polylines(polylines &p, double amount) const
{
    for (int i = p.data.size() - 1; i >= 0; i--)
    {
        vec3 dir = normalize(p.data[i].back() - p.data[i][0]);
        p.data[i][0] = p.data[i][0] + dir * amount;
        p.data[i].back() = p.data[i].back() - dir * amount;

        if (dot(p.data[i].back() - p.data[i][0], dir) < 0)
        {
            p.data.erase(p.data.begin() + i);
        }
    }
}

std::vector<section> infill_generator::generate_relevant_sections_angle(const plane &plane, double h, double angle, double space_between) const
{
    std::vector<section> result;

    std::array<section, 2> generators = create_generator_sections(plane, angle);

    double generator_length = (generators[0].p1 - generators[0].p2).length();

    double ray_count = generator_length / space_between;

    for (int i = 1; i < ray_count; i++)
    {
        opt_return<section> s = generate_relevant_section(plane, section((generators[0].p1 * (i / ray_count) + generators[0].p2 * (1 - i / ray_count)), (generators[1].p1 * (i / ray_count) + generators[1].p2 * (1 - i / ray_count))));
        if (s.valid)
        {
            result.push_back(s.data);
        }
    }
    return result;
}

polylines infill_generator::fit_polylines_to_marked_points(const std::vector<marked_point> &v) const
{
    polylines result;

    for (int i = 1; i < v.size(); i++)
    {
        //  itt mas lehetosegek is lehetnek
        if (v[i - 1].enters && !v[i].enters)
        {
            result.eat(std::vector<vec3>{v[i - 1], v[i]});
        }
    }

    return result;
}

polylines infill_generator::trace_section(const section &s, double step_distance, double h, double wall_thickness) const
{

    std::vector<infill_generator::marked_point> zero_points = find_sign_changes(s, step_distance, h, wall_thickness);

    return fit_polylines_to_marked_points(zero_points);
}

bool infill_generator::inside_advanced(const vec3 &p, double wall_thickness) const
{
    bool use_scale = false;
    if (!use_scale)
    {
        return surface->inside(p);
    }

    frep_scale scaled_down(*surface, -0.3 * wall_thickness);
    return scaled_down.inside(p);
}

std::vector<infill_generator::marked_point> infill_generator::find_sign_changes(const section &s, double step_distance, double h, double wall_thickness) const
{
    std::vector<infill_generator::marked_point> result;

    double step_count = (s.p1 - s.p2).length() / step_distance;

    vec3 last_pos(s.p2.x, s.p2.y, h);
    bool last_was_in = inside_advanced(last_pos, wall_thickness);

    for (int i = 1; i < step_count; i++)
    {
        vec2 current_pos_tmp = (s.p1 * (i / step_count) + s.p2 * (1 - i / step_count));
        vec3 current_pos(current_pos_tmp.x, current_pos_tmp.y, h);
        bool this_is_in = inside_advanced(current_pos, wall_thickness);

        if (this_is_in != last_was_in)
        {
            vec3 zero_point = (current_pos + last_pos) / 2; // bisectionnel majd
            result.push_back(marked_point(zero_point, this_is_in));
        }
        last_pos = current_pos;
        last_was_in = this_is_in;
    }

    return result;
}

opt_return<section> infill_generator::generate_relevant_section(const plane &plane, const section &intersector) const
{
    std::vector<section> plane_sides = plane.get_sides();
    std::vector<vec2> result_points;
    // if we cast a scanner section exactly at the diagonal, it will itersect all 4 sides, resulting 2 rlevant_section at the same place
    for (int i = 0; i < 4; i++)
    {
        opt_return<vec2> inter = intersector.intersect(plane_sides[i]);
        if (inter.valid)
        {
            result_points.push_back(inter.data);
        }

        if (result_points.size() == 2)
        {
            return {section(result_points[0], result_points[1])};
        }
    }

    return opt_return<section>();
}

std::array<section, 2> infill_generator::create_generator_sections(const plane &plane, double angle) const
{
    vec2 center = plane.get_center();
    vec2 top_point(0, 0);
    double optimal_distance = plane.get_diagonal_size(); // at least diagonal/sqrt(2)
    top_point.x += optimal_distance;

    vec3 left_point_tmp = vec3(top_point.x, top_point.y, 0);
    vec3 right_point_tmp = vec3(top_point.x, top_point.y, 0);

    TransformPoint(left_point_tmp, RotationMatrix(angle - M_PI / 4, vec3(0, 0, 1)));
    TransformPoint(right_point_tmp, RotationMatrix(angle + M_PI / 4, vec3(0, 0, 1)));

    vec2 left_point(left_point_tmp);
    vec2 right_point(right_point_tmp);

    left_point = left_point + center;
    right_point = right_point + center;

    vec2 normal = (left_point - right_point);
    normal.Normalize();
    normal.Rotate();

    return std::array<section, 2>{section(left_point, right_point), section(left_point + normal * optimal_distance * 2, right_point + normal * optimal_distance * 2)};
}
