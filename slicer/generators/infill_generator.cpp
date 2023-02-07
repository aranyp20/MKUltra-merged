#include "infill_generator.h"

infill_generator::infill_generator(frep_object *_surface) : surface(_surface)
{
}

polylines infill_generator::generate(const plane &plane, double h, double angle_between, double space_between) const
{
    std::array<section, 2> generators = create_generator_sections(plane, M_PI / 5);

        generate_relevant_section(plane, section((generators[0].p1 + generators[0].p2) / 2, (generators[1].p1 + generators[1].p2) / 2));

    return polylines();
}

std::vector<section> infill_generator::generate_relevant_sections_angle(const plane &plane, double h, double angle, double space_between) const
{
}

opt_return<section> infill_generator::generate_relevant_section(const plane &plane, const section &intersector) const
{
    std::vector<section> plane_sides = plane.get_sides();
    std::vector<vec2> result_points;
    // if we cast a scanner section exactly at the diagonal, it will itersect all 4 sides, resulting 2 rlevant_section at the same place
    for (int i = 0; i < 4; i++)
    {
        opt_return inter = intersector.intersect(plane_sides[i]);
        if (inter.valid)
        {
            result_points.push_back(inter.data);
        }

        if (result_points.size() == 2)
        {
            std::cout << "Kiraly" << std::endl;
            return {section(result_points[0], result_points[1])};
        }
    }

    return opt_return<section>();
}

std::array<section, 2> infill_generator::create_generator_sections(const plane &plane, double angle) const
{
    vec2 center = plane.get_center();
    vec2 top_point = center;
    double optimal_distance = plane.get_diagonal_size(); // at least diagonal/sqrt(2)
    top_point.x += optimal_distance;
    std::cout << top_point << std::endl;
    vec3 left_point_tmp = vec3(top_point.x, top_point.y, 0);
    vec3 right_point_tmp = vec3(top_point.x, top_point.y, 0);

    TransformPoint(left_point_tmp, RotationMatrix(angle - M_PI / 4, vec3(center.x, center.y, 1)));
    TransformPoint(right_point_tmp, RotationMatrix(angle + M_PI / 4, vec3(center.x, center.y, 1)));

    vec2 left_point(left_point_tmp);
    vec2 right_point(right_point_tmp);

    vec2 normal = (left_point - right_point);
    normal.Normalize();
    normal.Rotate();

    return std::array<section, 2>{section(left_point, right_point), section(left_point + normal * optimal_distance * 2, right_point + normal * optimal_distance * 2)};
}
