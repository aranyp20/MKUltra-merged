#include "printable_object.h"

sliced_obj::sliced_obj(const std::vector<polylines> &_data, const plane &_bp) : data(_data), bounding_plane(_bp)
{

    for (int i = 0; i < _data.size(); i++)
    {
        float **current_level_raw = new float *[_data[i].data.size()];
        for (int j = 0; j < _data[i].data.size(); j++)
        {
            int float_count_in_line = (_data[i].data[j].size() * 2 - 2) * 3;
            if (float_count_in_line <= 0)
                continue;

            current_level_raw[j] = new float[float_count_in_line];

            int helper_it = 0;
            for (int k = 0; k < _data[i].data[j].size(); k++)
            {
                if (k > 0 && k < _data[i].data[j].size() - 1)
                {
                    current_level_raw[j][helper_it] = _data[i].data[j][k].x;
                    current_level_raw[j][helper_it + 1] = _data[i].data[j][k].y;
                    current_level_raw[j][helper_it + 2] = _data[i].data[j][k].z;

                    helper_it += 3;
                }
                current_level_raw[j][helper_it] = _data[i].data[j][k].x;
                current_level_raw[j][helper_it + 1] = _data[i].data[j][k].y;
                current_level_raw[j][helper_it + 2] = _data[i].data[j][k].z;

                helper_it += 3;
            }

            std::cout << float_count_in_line << " " << helper_it << std::endl;
        }
        raw_datas.push_back(current_level_raw);
    }

    for (auto &a : data)
    {
        polylines::iterator it = a.begin();
        while (it.has_next())
        {
            auto tmp = (it.get_data());

            // if (tmp != nullptr)
        }
    }

    slice_count = data.size();
}
