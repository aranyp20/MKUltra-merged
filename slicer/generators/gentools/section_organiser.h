#pragma once

#include <functional>
#include <vector>
#include <map>

#include "interval.h"
#include "basics.hpp"
#include "section.h"

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