#pragma once

#include <frep.hpp>
#include <basics.hpp>
#include "plane.h"

class infill_generator
{
    frep_object *surface;

    std::array<section, 2> create_generator_sections(const plane &, double angle) const;
    std::vector<section> generate_relevant_sections_angle(const plane &, double h, double angle, double space_between) const;
    std::vector<section> generate_relevant_sections(const plane &, double h, double angle_between, double space_between) const;
    opt_return<section> generate_relevant_section(const plane &, const section &intersector) const;

public:
    infill_generator(frep_object *);

    polylines generate(const plane &, double h, double angle_between, double space_between) const;
};