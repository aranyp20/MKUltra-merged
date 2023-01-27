#pragma once

#include <functional>
#include <vector>
#include <map>

#include <interval.h>
#include <basics.h>
#include <frep.hpp>

class slicer{


   

    struct square{

        //       0
        //    0-----1
        //  3 |     | 1
        //    3-----2
        //       2


        vec2 start; //upper-left pos
        double size;

        square(const vec2&,double);

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
    
    //std::function<interval(const interval&, const interval&, double h)> frep_interval;

    struct id_section{
        section me;
        unsigned int start_id;
        unsigned int end_id;

        id_section(const section&, unsigned int start_id, unsigned int end_id);
    };

    class section_indexer{
    public:
        std::vector<section> found;
        bool same_section(const section&, const section&) const;
        
        unsigned int find_add_section(const section&);
    };

    std::vector<slicer::square> rejection_testing(const square&,double h,unsigned int resolution) const;
    bool rejection_test(const square&, double h) const;
    std::vector<id_section> generate_contour(const std::vector<square>& unrejecteds, double h) const;
    std::vector<bool> evaluate_verts(const square&, double h) const;
    vec2 calc_surfacepoint(const section&, double h) const;
    std::vector<std::vector<section>> organise_sections(const std::vector<id_section>&) const;

    void print(const std::vector<section>&) const;
public:

    //slicer(std::function<interval(const interval&, const interval&, double h)> f_interval);
    slicer();
    
    void slice(double h, unsigned int resolution) const;
};