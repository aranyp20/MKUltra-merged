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

sliced_object::sliced_object(const std::vector<layer_data> &_data, const plane &_bp) : data(_data), bounding_plane(_bp)
{

    slice_count = data.size();
}

void sliced_object::layer_data::normalize_for_gl(vec3 &p, const plane &plane) const
{
    vec2 changed_part(p);

    changed_part = changed_part - plane.get_center();

    changed_part.x = 2.0f * changed_part.x / plane.get_size();
    changed_part.y = 2.0f * changed_part.y / plane.get_size();

    p = vec3(changed_part.x, changed_part.y, p.z);
}

const std::vector<float> &sliced_object::get_org_level(unsigned int level) const
{
    return data[level].combined_org;
}

const std::vector<float> &sliced_object::get_org_level(unsigned int level, const layer_data::part_type &pt) const
{
    return data[level].parts[pt].org;
}

void sliced_object::set_level_color(const std::vector<qgl_vertex> &_colored, unsigned int level, const layer_data::part_type &pt)
{
    data[level].parts[pt].colored = _colored;
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

sliced_object::layer_data::layer_data(const polylines &_outer, const polylines &_inner, const polylines &_infill, const plane &_bp)
{
    parts[0].poly = _inner;
    parts[0].org = transfer(_inner, _bp);

    parts[1].poly = _outer;
    parts[1].org = transfer(_outer, _bp);

    parts[2].poly = _infill;
    parts[2].org = transfer(_infill, _bp);

    combine_orgs();
}

std::vector<float> sliced_object::layer_data::transfer(const polylines &_data, const plane &_bp) const
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
            normalize_for_gl(current_point, _bp);

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
