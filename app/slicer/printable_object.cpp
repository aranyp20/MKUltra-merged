#include "printable_object.h"

sliced_obj::sliced_obj(const std::vector<polylines> &_data) : data(_data)
{

    for (auto &a : data)
    {
        polylines::iterator it = a.begin();
        while (it.has_next())
        {
            auto tmp = (it.get_data());

            if (tmp != nullptr)
                check_bounds(*tmp);
        }
    }

    slice_count = data.size();
}

void sliced_obj::check_bounds(const vec3 &p)
{
    x_boundaries[0] = std::min(x_boundaries[0], p.x);
    x_boundaries[1] = std::max(x_boundaries[1], p.x);

    y_boundaries[0] = std::min(y_boundaries[0], p.y);
    y_boundaries[1] = std::max(y_boundaries[1], p.y);

    h_boundaries[0] = std::min(h_boundaries[0], p.z);
    h_boundaries[1] = std::max(h_boundaries[1], p.z);
}
