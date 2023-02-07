#include "infill_generator.h"

infill_generator::infill_generator(frep_object *_surface) : surface(_surface)
{
}

polylines infill_generator::generate(const plane &plane, double h, double angle_between, double space_between) const
{
}

std::vector<section> infill_generator::generate_relevant_sections_angle(const plane &plane, double h, double angle, double space_between) const
{
}

section infill_generator::generate_relevant_section(const plane &plane, const section &intersector) const
{
}

std::array<section, 2> infill_generator::create_generator_sections(const plane &plane, double h, double angle) const
{
    vec2 center = plane.get_center();
    vec2 top_point = center;
    top_point.x += plane.get_diagonal_size() / 2;
    vec3 left_point_tmp = vec3(top_point.x, top_point.y, 0);
    vec3 right_point_tmp = vec3(top_point.x, top_point.y, 0);

    TransformPoint(left_point_tmp, RotationMatrix(angle - M_PI / 4, vec3(center.x, center.y, 1)));
    TransformPoint(right_point_tmp, RotationMatrix(angle + M_PI / 4, vec3(center.x, center.y, 1)));

    vec2 left_point(left_point_tmp);
    vec2 right_point(right_point_tmp);

    // lehet frditva
    vec2 normal = left_point - right_point;
    normal.Rotate();

    return std::array<section, 2>{section(left_point, right_point), section(left_point + normal * plane.get_diagonal_size(), right_point + normal * plane.get_diagonal_size())};
}
