#pragma once

#include <vector>
#include <mymath.h>




struct section{

    vec2 p1,p2;

    section(const vec2& _p1, const vec2& _p2) : p1(_p1), p2(_p2){}

    inline double length() const {return (p2-p1).length();}
    inline void swap_sides() {
        std::swap(p1,p2);
    }

};

struct section_table{

    std::vector<std::vector<section>> data;

    struct iterator{
        
        std::vector<std::vector<section>>& data;

        bool has_next();
        section* get_section() const;
    
        iterator(std::vector<std::vector<section>>&);

    private:
    
        unsigned int it_outer = 0;
        unsigned int it_inner = 0;


    };

    iterator begin();

    section_table(){}
    section_table(const std::vector<std::vector<section>>&);

};