#pragma once

#include <vector>

#include <basics.hpp>
#include <frep.hpp>


class inner_shell_generator{


public:

    bool detect_intersection(const vec3&, polylines& contour, double w) const; 
    vec3 offset_p(const vec3&, double w, double h) const;
    std::vector<vec3> generate_one_part(polylines& contour, unsigned int which_part, double w, double h) const;
    polylines generate_one(polylines& contour, double w, double h) const; 

};