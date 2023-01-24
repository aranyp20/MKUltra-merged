#pragma once

#include <functional>
#include <vector>
#include <map>

#include "interval.h"
#include "basics.h"

class slicer{


    struct square{

        //       0
        //    0-----1
        //  3 |     | 1
        //    3-----2
        //       2


        vec2 start; //upper-left pos
        double size;

        square(vec2,double);

        std::vector<square> breakup(unsigned int count /*row and column count*/) const;
        section get_section(unsigned int side_index) const;
        vec2 get_vert(unsigned int vert_index) const;
    }; 

    class LUT{
        static std::map<std::vector<bool>,std::vector<std::pair<unsigned int, unsigned int>>> table;
    public:
        /*clockwise, starting from upper-left corner*/
        static std::vector<std::pair<unsigned int,unsigned int>> find_intersectables(const std::vector<bool>&);
    };
    
    std::function<interval(const interval&, const interval&, double h)> frep_interval;
    

    std::vector<slicer::square> rejection_testing(const square&,double h,unsigned int resolution) const;
    bool rejection_test(const square&, double h) const;
    std::vector<section> generate_contour(const std::vector<square>& unrejecteds, double h) const;
    std::vector<section> generate_sections(const std::pair<unsigned int, unsigned int>&) const;
    vec2 calc_surfacepoint(const section&, double h) const;

    void print(const std::vector<section>&) const;
public:

    slicer(std::function<interval(const interval&, const interval&, double h)> f_interval);
    
    void slice(double h, unsigned int resolution) const;
};