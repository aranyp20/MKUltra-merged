#pragma once

#include <array>
#include <float.h>
#include <QVector3D>
#include "polylines.h"
#include "frep.hpp"

struct qgl_vertex
{
    QVector3D position;
    QVector3D color;
};

class sliced_object
{
public:
    struct layer_data
    {
        enum part_type
        {
            INNER,
            OUTER,
            INFILL
        };

        struct part
        {
            polylines poly;
            std::vector<float> org;
            std::vector<qgl_vertex> colored;
            std::vector<qgl_vertex> colored_separated;
        };

        std::array<part, 3> parts;
        std::vector<float> combined_org;
        std::vector<qgl_vertex> combined_colored;
        std::vector<qgl_vertex> combined_colored_separated;
        void combine_orgs();
        void combine_coloreds();
        void combine_coloreds_separated();

        layer_data(const polylines &_outer, const polylines &_inner, const polylines &_infill, const bounding_box &_bb, bool is_suport = false);

        void normalize_for_gl(vec3 &, const bounding_box &) const;
        std::vector<float> transfer(const polylines &_data, const bounding_box &_bb = bounding_box(vec3(-1, -1, -1), 2, 2)) const;
        std::vector<qgl_vertex> colorize(const std::vector<float> &, const vec3 &_color) const;
        std::vector<qgl_vertex> colorize(const std::vector<float> &positions, const std::vector<float> &colors) const;

        void eat(const layer_data &);
    };

private:
    std::vector<layer_data> data;

    bounding_box m_bounding_box;
    unsigned int slice_count;

    void eat(const sliced_object &);

public:
    unsigned int get_slice_count() const;

    sliced_object() = default;
    sliced_object(const std::vector<layer_data> &, const bounding_box &_bb);
    sliced_object(const sliced_object &base, const sliced_object &addable);

    const layer_data &get_level(unsigned int) const;
    const std::vector<float> &get_org_level(unsigned int) const;
    const std::vector<float> &get_org_level(unsigned int, const layer_data::part_type &) const;
    const std::vector<qgl_vertex> &get_colored_level(unsigned int) const;
    const std::vector<qgl_vertex> &get_colored_separated_level(unsigned int) const;


    const std::vector<qgl_vertex> get_colored() const;

    const polylines &get_poly_level(unsigned int, const layer_data::part_type &) const;
    const polylines get_poly_level(unsigned int) const;

    const std::vector<qgl_vertex> get_custom_colored_level(unsigned int, const vec3 &) const;

    void set_level_color(const std::vector<qgl_vertex> &, unsigned int, const layer_data::part_type &);
    void set_level_color(const polylines &colors, unsigned int, const layer_data::part_type &);
};