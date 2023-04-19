#pragma once

#include <vector>
#include <memory>
#include <basics.hpp>
#include <frep.hpp>

class inner_shell_generator
{
    std::shared_ptr<frep_object> surface;

    void generate_one_point(polylines &result, polylines &contour, const vec3 &from, std::vector<vec3> &current_string, vec3 &last_pos, double w, bool &active) const;
    void point_was_bad(const vec3 &p, std::vector<vec3> &current_string, vec3 &last_pos, bool &active, polylines &result) const;
    void point_was_good(const vec3 &p, std::vector<vec3> &current_string, vec3 &last_pos, bool &active) const;
    bool detect_intersection(const vec3 &, polylines &contour, double w) const;
    vec3 offset_p(const vec3 &, double w) const;
    polylines generate_one_part(polylines &contour, unsigned int which_part, double w) const;

public:
    inner_shell_generator(std::shared_ptr<frep_object>);

    polylines generate_one(polylines &contour, double w) const;
};