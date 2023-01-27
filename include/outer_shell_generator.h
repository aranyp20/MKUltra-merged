#pragma once

#include <functional>
#include <vector>
#include <map>

#include <interval.h>
#include <basics.h>
#include <frep.hpp>

struct id_section{
    section me;
    unsigned int id_a;
    unsigned int id_b;
    
    id_section(const section&, unsigned int, unsigned int);

    void swap();
};


class section_organiser{

    void find_attachment(std::vector<id_section>& from,std::vector<id_section>& current_string) const;
    bool try_attach(std::vector<id_section>& from,std::vector<id_section>& current_string, bool to_end) const;
    void attach(std::vector<id_section>& from,unsigned int what,std::vector<id_section>& current_string, bool to_end) const;
    bool try_join_ends(std::vector<id_section>&) const;

public:

    std::vector<std::vector<section>> organise_sections(const std::vector<id_section>&) const;
    
};


class outer_shell_generator{
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
    

    class section_indexer{
        std::vector<section> found;
        bool same_section(const section&, const section&) const;
    public:
        
        unsigned int PUT_section(const section&);
    };

    std::vector<outer_shell_generator::square> rejection_testing(const square&,double h,unsigned int resolution) const;
    bool rejection_test(const square&, double h) const;
    std::vector<id_section> generate_contour(const std::vector<square>& unrejecteds, double h) const;
    std::vector<bool> evaluate_verts(const square&, double h) const;
    vec2 calc_surfacepoint(const section&, double h) const;

    section_organiser organiser;

public:

    outer_shell_generator();
    
    std::vector<std::vector<section>> generate(double h, unsigned int resolution) const;
};