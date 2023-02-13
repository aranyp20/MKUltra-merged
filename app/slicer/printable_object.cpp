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
                if (k > 0 && k < _data[i].data[j].size() - 1)
                {
                    current_level.push_back((float)_data[i].data[j][k].x);
                    current_level.push_back((float)_data[i].data[j][k].y);
                    current_level.push_back((float)_data[i].data[j][k].z);
                }
                current_level.push_back((float)_data[i].data[j][k].x);
                current_level.push_back((float)_data[i].data[j][k].y);
                current_level.push_back((float)_data[i].data[j][k].z);
            }
        }

        filled_data.push_back(current_level);
    }

    slice_count = data.size();
}
