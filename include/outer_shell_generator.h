#pragma once

#include <functional>
#include <vector>
#include <map>

#include <interval.h>
#include <basics.hpp>
#include <frep.hpp>

struct id_section
{
    section me;
    unsigned int id_a;
    unsigned int id_b;

    id_section(const section &, unsigned int, unsigned int);

    void swap();
};

class section_organiser
{
    bool attach_to_end(std::vector<id_section> &from, std::vector<id_section> &current_string, unsigned int id) const;
    bool attach_to_front(std::vector<id_section> &from, std::vector<id_section> &current_string, unsigned int id) const;
    void push_back_idsections(polylines &result, std::vector<id_section> &current_string, double h) const;
    void create_one_string(polylines &result, std::vector<id_section> &unorganised, double h) const;
    void find_attachments(std::vector<id_section> &from, std::vector<id_section> &current_string) const;
    bool try_attach(std::vector<id_section> &from, std::vector<id_section> &current_string, bool to_end) const;
    void attach(std::vector<id_section> &from, unsigned int what, std::vector<id_section> &current_string, bool to_end) const;
    bool try_join_ends(std::vector<id_section> &) const;

public:
    polylines organise_sections(const std::vector<id_section> &, double h) const;
};

class outer_shell_generator
{
    frep_object *surface;

    struct square
    {

        //       0
        //    0-----1
        //  3 |     | 1
        //    3-----2
        //       2
        vec2 start; // upper-left pos
        double size;

        square(const vec2 &, double);

        std::vector<square> breakup(unsigned int count /*row and column count*/) const;
        section get_section(unsigned int side_index) const;
        vec2 get_vert(unsigned int vert_index) const;
    };

    class LUT
    {
        static std::map<std::vector<bool>, std::vector<std::array<unsigned int, 2>>> table;

    public:
        /*clockwise, starting from upper-left corner*/
        static std::vector<std::array<unsigned int, 2>> find_intersectables(const std::vector<bool> &);
    };

    class section_indexer
    {
        std::vector<section> found;
        bool same_section(const section &, const section &) const;

    public:
        unsigned int PUT_section(const section &);
    };

    std::vector<std::array<unsigned int, 2>> find_section_indexes(const square &sq, section_indexer &indexer, const std::vector<std::array<unsigned int, 2>>) const;
    std::vector<std::array<section, 2>> find_sections_to_ids(const square &sq, const std::vector<std::array<unsigned int, 2>> &ids) const;
    void square_to_idsections_for_contour(const square &sq, std::vector<id_section> &result, section_indexer &indexer, double h) const;
    vec2 find_zero_value_point(const std::array<vec2, 2> &betweens, double h) const;
    std::array<vec2, 2> arrange_inside_outside_point(const section &se, double h) const;
    void rejection_testing_square(std::vector<square> &leaves, const square &testable, double h, unsigned int resolution) const;
    std::vector<outer_shell_generator::square> rejection_testing(const square &, double h, unsigned int resolution) const;
    bool rejection_test(const square &, double h) const;
    std::vector<id_section> generate_contour(const std::vector<square> &unrejecteds, double h) const;
    std::vector<bool> evaluate_verts(const square &, double h) const;
    vec2 calc_surfacepoint(const section &, double h) const;

    section_organiser organiser;

public:
    outer_shell_generator(frep_object *);

    polylines generate(const std::pair<vec2, double> &bounding_box, double h, unsigned int resolution) const;
};