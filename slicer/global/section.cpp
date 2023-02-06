#include <section.h>

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