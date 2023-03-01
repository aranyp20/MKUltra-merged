#pragma once

#include <array>
#include <float.h>
#include <QVector3D>
#include "polylines.h"
#include "plane.h"

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
        };

        std::array<part, 3> parts;
        std::vector<float> combined_org;
        std::vector<qgl_vertex> combined_colored;
        void combine_orgs();
        void combine_coloreds();

        layer_data(const polylines &_outer, const polylines &_inner, const polylines &_infill, const plane &_bp);

    private:
        void normalize_for_gl(vec3 &, const plane &) const;
        std::vector<float> transfer(const polylines &_data, const plane &_bp) const;
        std::vector<qgl_vertex> colorize(const std::vector<float>&, const vec3& _color) const;
    };

private:
    std::vector<layer_data> data;

    plane bounding_plane;
    unsigned int slice_count;

public:
    unsigned int get_slice_count() const;
    // const std::vector<std::vector<float>> &get_org_data() const;

    sliced_object(const std::vector<layer_data> &, const plane &_bp);

    const std::vector<float> &get_org_level(unsigned int) const;
    const std::vector<float> &get_org_level(unsigned int, const layer_data::part_type &) const;
    const std::vector<qgl_vertex> &get_colored_level(unsigned int) const;

    const std::vector<qgl_vertex> get_colored() const;

    void set_level_color(const std::vector<qgl_vertex> &, unsigned int, const layer_data::part_type &);
};