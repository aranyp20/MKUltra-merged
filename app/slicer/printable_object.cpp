#include "printable_object.h"

sliced_obj::sliced_obj(const std::vector<polylines> &_data, const plane &_bp) : data(_data), bounding_plane(_bp)
{

    for (int i = 0; i < _data.size(); i++)
    {
        // float *current_level_raw = new float *[_data[i].data.size()];

        std::vector<float> current_level;

        for (int j = 0; j < _data[i].data.size(); j++)
        {

            int float_count_in_line = (_data[i].data[j].size() * 2 - 2) * 3;
            if (float_count_in_line <= 0)
            {
                continue;
            }

            for (int k = 0; k < _data[i].data[j].size(); k++)
            {
                vec3 current_point(_data[i].data[j][k]);
                normalize_for_gl(current_point, _bp);

                if (k > 0 && k < _data[i].data[j].size() - 1)
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

        filled_data.push_back(current_level);
    }

    slice_count = data.size();
}

void sliced_obj::normalize_for_gl(vec3 &p, const plane &plane) const
{
    vec2 changed_part(p);

    changed_part = changed_part - plane.get_center();

    changed_part.x = 2.0f * changed_part.x / plane.get_size();
    changed_part.y = 2.0f * changed_part.y / plane.get_size();

    p = vec3(changed_part.x, changed_part.y, p.z);
}
