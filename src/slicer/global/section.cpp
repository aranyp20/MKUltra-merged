#include "section.h"

section::section(const vec2 &_p1, const vec2 &_p2) : p1(_p1), p2(_p2) {}

double section::length() const
{
    return (p2 - p1).length();
}
void section::swap_sides()
{
    std::swap(p1, p2);
}

std::vector<vec3> serialize(const std::vector<section> &v, double h)
{
    std::vector<vec3> result;
    for (const auto &a : v)
    {
        result.push_back(vec3(a.p1.x, a.p1.y, h));
    }
    result.push_back(vec3(v.back().p2.x, v.back().p2.y, h));

    return result;
}

opt_return<vec2> section::intersect(const section &other) const
{
    vec2 DIR = p2 - p1;
    vec2 START = p1;

    vec2 normal2(other.p1 - other.p2);
    normal2.Rotate();

    if (FE(dot(normal2, DIR), 0))
    {
        return opt_return<vec2>();
    }

    float t1 = -1 * dot(normal2, START - other.p2) / dot(normal2, DIR);
    vec2 line_intersection(START + DIR * t1);

    return {line_intersection, dot(line_intersection - other.p1, line_intersection - other.p2) < 0 && dot(line_intersection - p1, line_intersection - p2) < 0};
}
