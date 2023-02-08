#include "plane.h"

plane::plane(const vec2 &_corner, double _size) : corner(_corner), size(_size)
{
}

plane::plane(const std::pair<vec2, double> &d) : corner(d.first), size(d.second) {}

vec2 plane::get_center() const
{
    return vec2(corner.x + size / 2, corner.y + size / 2);
}
double plane::get_diagonal_size() const
{
    return (get_center() - corner).length() * 2;
}

std::vector<section> plane::get_sides() const
{
    vec2 p1(corner.x, corner.y + size);
    vec2 p2(corner.x + size, corner.y + size);
    vec2 p3(corner.x + size, corner.y);
    vec2 p4(corner);
    return std::vector<section>{section(p1, p2), section(p2, p3), section(p3, p4), section(p4, p1)};
}

double plane::get_size() const
{
    return size;
}
