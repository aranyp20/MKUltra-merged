#pragma once

#include <vector>

#include <basics.h>
#include <frep.hpp>


class inner_shell_generator{


public:

    bool detect_intersection(const vec2&, const section_table& contour, double w) const; 
    vec2 offset_p(const vec2&, double w, double h) const;
    std::vector<section> generate_one_part(const section_table& contour, unsigned int which_part, double w, double h) const;
    section_table generate_one(const section_table& contour, double w, double h) const; 

};