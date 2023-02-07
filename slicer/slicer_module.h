#pragma once

#include "outer_shell_generator.h"
#include "inner_shell_generator.h"
#include "infill_generator.h"

class slicer
{
public:
    struct bounding_box
    {
        std::pair<vec3, double> floor;
        double height;

        bounding_box(const vec3 &_corner, double _width, double _height);
    };

    slicer(frep_object *_cutable_obj, const bounding_box &_box);

    std::vector<polylines> slice(double h_per_max, unsigned int inner_shell_count, double inner_shell_distance) const;
    void create_slices(unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance) const;

private:
    frep_object *cutable_obj;
    bounding_box my_bounding_box;

    outer_shell_generator outer_generator;
    inner_shell_generator inner_generator;
    infill_generator inf_generator;
};