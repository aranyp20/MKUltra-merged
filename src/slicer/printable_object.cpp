#include "printable_object.h"

unsigned int sliced_object::get_slice_count() const
{
    return slice_count;
}
/*
const std::vector<std::vector<float>> &sliced_object::get_org_data() const
{
    return org_data;
}
*/

sliced_object::sliced_object(const std::vector<layer_data> &_data, const bounding_box &_bb) : data(_data), m_bounding_box(_bb)
{

    slice_count = data.size();
}

sliced_object::sliced_object(const sliced_object &base, const sliced_object &addable) : sliced_object(base)
{
    eat(addable);
}

void sliced_object::layer_data::normalize_for_gl(vec3 &p, const bounding_box &box) const
{
    vec3 changed(p);

    changed = changed - box.get_center();

    double divider = std::max(box.get_size(), box.get_height());

    changed = (changed * 2) / divider;

    p = changed;
}

const sliced_object::layer_data &sliced_object::get_level(unsigned int i) const
{
    return data[i];
}

const std::vector<float> &sliced_object::get_org_level(unsigned int level) const
{
    return data[level].combined_org;
}

const std::vector<float> &sliced_object::get_org_level(unsigned int level, const layer_data::part_type &pt) const
{
    return data[level].parts[pt].org;
}

const std::vector<qgl_vertex> &sliced_object::get_colored_level(unsigned int level) const
{
    return data[level].combined_colored;
}

const std::vector<qgl_vertex> &sliced_object::get_colored_separated_level(unsigned int level) const
{
    return data[level].combined_colored_separated;
}

const std::vector<qgl_vertex> sliced_object::get_colored() const
{
    std::vector<qgl_vertex> result;
    for (int i = 0; i < data.size(); i++)
    {
        std::vector<qgl_vertex> c_level = get_colored_level(i);
        result.insert(result.end(), c_level.begin(), c_level.end());
    }
    return result;
}

const polylines &sliced_object::get_poly_level(unsigned int level, const layer_data::part_type &part) const
{
    return data[level].parts[part].poly;
}

const polylines sliced_object::get_poly_level(unsigned int level) const
{
    polylines result;

    for (const auto &b : data[level].parts)
    {
        result.add_together(b.poly);
    }

    return result;
}

const std::vector<qgl_vertex> sliced_object::get_custom_colored_level(unsigned int level, const vec3 &color) const
{
    return data[level].colorize(data[level].combined_org, color);
}

void sliced_object::set_level_color(const std::vector<qgl_vertex> &_colored, unsigned int level, const layer_data::part_type &pt)
{
    data[level].parts[pt].colored = _colored;

    data[level].combine_coloreds();
}

void sliced_object::set_level_color(const polylines &colors, unsigned int level, const layer_data::part_type &pt)
{
    data[level].parts[pt].colored = data[level].colorize(data[level].parts[pt].org, data[level].transfer(colors));

    data[level].combine_coloreds();
}

void sliced_object::layer_data::combine_orgs()
{
    std::vector<float> result;

    for (int i = 0; i < parts.size(); i++)
    {
        result.insert(result.end(), parts[i].org.begin(), parts[i].org.end());
    }
    combined_org = result;
}

void sliced_object::layer_data::combine_coloreds()
{
    std::vector<qgl_vertex> result;

    for (int i = 0; i < parts.size(); i++)
    {
        result.insert(result.end(), parts[i].colored.begin(), parts[i].colored.end());
    }

    combined_colored = result;
}

void sliced_object::layer_data::combine_coloreds_separated()
{
    std::vector<qgl_vertex> result;

    for (int i = 0; i < parts.size(); i++)
    {
        result.insert(result.end(), parts[i].colored_separated.begin(), parts[i].colored_separated.end());
    }

    combined_colored_separated = result;
}

sliced_object::layer_data::layer_data(const polylines &_outer, const polylines &_inner, const polylines &_infill, const bounding_box &_bb, bool is_support)
{
    if (!is_support)
    {
        parts[part_type::INNER].poly = _inner;
        parts[part_type::INNER].org = transfer(_inner, _bb);
        parts[part_type::INNER].colored = colorize(parts[part_type::INNER].org, vec3(0, 0, 1));

        parts[part_type::OUTER].poly = _outer;
        parts[part_type::OUTER].org = transfer(_outer, _bb);
        parts[part_type::OUTER].colored = colorize(parts[part_type::OUTER].org, vec3(0, 0, 0));

        parts[part_type::INFILL].poly = _infill;
        parts[part_type::INFILL].org = transfer(_infill, _bb);
        parts[part_type::INFILL].colored = colorize(parts[part_type::INFILL].org, vec3(0, 1, 0));
    }
    else
    {
        parts[part_type::INNER].poly = _inner;
        parts[part_type::INNER].org = transfer(_inner, _bb);
        parts[part_type::INNER].colored = colorize(parts[part_type::INNER].org, vec3(1, 0, 1));

        parts[part_type::OUTER].poly = _outer;
        parts[part_type::OUTER].org = transfer(_outer, _bb);
        parts[part_type::OUTER].colored = colorize(parts[part_type::OUTER].org, vec3(1, 0, 1));

        parts[part_type::INFILL].poly = _infill;
        parts[part_type::INFILL].org = transfer(_infill, _bb);
        parts[part_type::INFILL].colored = colorize(parts[part_type::INFILL].org, vec3(1, 0, 1));
    }

    if (is_support)
    {
        parts[part_type::INNER].colored_separated = colorize(parts[part_type::INNER].org, vec3(1, 0, 0));
        parts[part_type::OUTER].colored_separated = colorize(parts[part_type::OUTER].org, vec3(1, 0, 0));
        parts[part_type::INFILL].colored_separated = colorize(parts[part_type::INFILL].org, vec3(1, 0, 0));
    }
    else
    {
        parts[part_type::INNER].colored_separated = colorize(parts[part_type::INNER].org, vec3(0, 1, 0));
        parts[part_type::OUTER].colored_separated = colorize(parts[part_type::OUTER].org, vec3(0, 1, 0));
        parts[part_type::INFILL].colored_separated = colorize(parts[part_type::INFILL].org, vec3(0, 1, 0));
    }

    combine_orgs();
    combine_coloreds();
    combine_coloreds_separated();
}

std::vector<float> sliced_object::layer_data::transfer(const polylines &_data, const bounding_box &_bb) const
{
    std::vector<float> current_level;

    for (int j = 0; j < _data.data.size(); j++)
    {

        int float_count_in_line = (_data.data[j].size() * 2 - 2) * 3;
        if (float_count_in_line <= 0)
        {
            continue;
        }

        for (int k = 0; k < _data.data[j].size(); k++)
        {
            vec3 current_point(_data.data[j][k]);
            normalize_for_gl(current_point, _bb);

            if (k > 0 && k < _data.data[j].size() - 1)
            {
                current_level.push_back((float)current_point.x);
                current_level.push_back((float)current_point.y);
                current_level.push_back((float)current_point.z);
            }
            current_level.push_back((float)current_point.x);
            current_level.push_back((float)current_point.y);
            current_level.push_back((float)current_point.z);
        }
    }

    return current_level;
}

std::vector<qgl_vertex> sliced_object::layer_data::colorize(const std::vector<float> &_data, const vec3 &_color) const
{
    std::vector<qgl_vertex> result;

    for (int i = 0; i < _data.size(); i += 3)
    {
        result.push_back({QVector3D(_data[i], _data[i + 1], _data[i + 2]), QVector3D(_color.x, _color.y, _color.z)});
    }

    return result;
}

std::vector<qgl_vertex> sliced_object::layer_data::colorize(const std::vector<float> &positions, const std::vector<float> &colors) const
{
    std::vector<qgl_vertex> result;

    for (int i = 0; i < positions.size(); i += 3)
    {
        result.push_back({QVector3D(positions[i], positions[i + 1], positions[i + 2]), QVector3D(colors[i], colors[i + 1], colors[i + 2])});
    }

    return result;
}

void sliced_object::eat(const sliced_object &other)
{
    if (other.get_slice_count() != get_slice_count())
    {
        std::cout << "Two different slice_count." << std::endl;
        return;
    }

    for (unsigned int i = 0; i < get_slice_count(); i++)
    {
        data[i].eat(other.get_level(i));
    }
}

void sliced_object::layer_data::eat(const layer_data &other)
{
    for (unsigned int i = 0; i < parts.size(); i++)
    {
        parts[i].poly.add_together(other.parts[i].poly);
        parts[i].org.insert(parts[i].org.end(), other.parts[i].org.begin(), other.parts[i].org.end());
        parts[i].colored.insert(parts[i].colored.end(), other.parts[i].colored.begin(), other.parts[i].colored.end());
        parts[i].colored_separated.insert(parts[i].colored_separated.end(), other.parts[i].colored_separated.begin(), other.parts[i].colored_separated.end());
    }
    combine_orgs();
    combine_coloreds();
    combine_coloreds_separated();
}
