#pragma once

#include <frep.hpp>
#include <basics.hpp>
#include "plane.h"

class infill_generator
{
    frep_object *surface;

    // prevents small errors causing big problems...
    struct marked_point : public vec3
    {
        bool enters;
        marked_point(const vec3 &, bool);
    };

    std::array<section, 2> create_generator_sections(const plane &, double angle) const;
    std::vector<section> generate_relevant_sections_angle(const plane &, double h, double angle, double space_between) const;
    std::vector<section> generate_relevant_sections(const plane &, double h, double angle_between, double space_between) const;
    opt_return<section> generate_relevant_section(const plane &, const section &intersector) const;
    polylines trace_section(const section &, double step_distance, double h) const;
    std::vector<marked_point> find_sign_changes(const section &, double step_distance, double h) const; // return is ordered
    polylines fit_polylines_to_marked_points(const std::vector<marked_point> &) const;
    void shrink_polylines(polylines &, double amount) const;
    polylines generate_one(const plane &plane, double h, double angle, double space_between, double wall_thickness) const;

public:
    infill_generator(frep_object *);

    polylines generate(const plane &, double h, double angle_between, double space_between, double wall_thickness) const;
};