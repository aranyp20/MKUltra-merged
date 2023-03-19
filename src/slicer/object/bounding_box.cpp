#include "bounding_box.h"

bounding_box::bounding_box(const vec3 &_corner, double _width, double _height) : floor(std::pair<vec3, double>{_corner, _width}), height(_height) {}

vec3 bounding_box::get_center() const
{
    return vec3(floor.first.x + (floor.second / 2), floor.first.y + (floor.second / 2), floor.first.z + (height / 2));
}

double bounding_box::get_size() const { return floor.second; }

double bounding_box::get_height() const { return height; }
