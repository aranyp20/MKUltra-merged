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

void sliced_object::layer_data::normalize_for_gl(vec3 &p, const bounding_box &box) const
{
    vec3 changed(p);

    changed = changed - box.get_center();

    double divider = std::max(box.get_size(), box.get_height());

    changed = (changed * 2) / divider;

    p = changed;
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

const std::vector<qgl_vertex> sliced_object::get_custom_colored_level(unsigned int level, const vec3 &color) const
{
    return data[level].colorize(data[level].combined_org, color);
}

void sliced_object::set_level_color(const std::vector<qgl_vertex> &_colored, unsigned int level, const layer_data::part_type &pt)
{
    data[level].parts[pt].colored = _colored;

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

sliced_object::layer_data::layer_data(const polylines &_outer, const polylines &_inner, const polylines &_infill, const bounding_box &_bb)
{
    parts[0].poly = _inner;
    parts[0].org = transfer(_inner, _bb);
    parts[0].colored = colorize(parts[0].org, vec3(0, 0, 1));

    parts[1].poly = _outer;
    parts[1].org = transfer(_outer, _bb);
    parts[1].colored = colorize(parts[1].org, vec3(0, 1, 0));

    parts[2].poly = _infill;
    parts[2].org = transfer(_infill, _bb);
    parts[2].colored = colorize(parts[2].org, vec3(1, 0, 0));

    combine_orgs();
    combine_coloreds();
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
