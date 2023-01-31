#include <inner_shell_generator.h>

polylines inner_shell_generator::generate_one(polylines &contour, double w) const
{
    polylines result;

    for (int i = 0; i < contour.data.size(); i++)
    {
        result.data.push_back(generate_one_part(contour, i, w));
    }

    return result;
}

std::vector<vec3> inner_shell_generator::generate_one_part(polylines &contour, unsigned int which_part, double w) const
{
    std::vector<vec3> result;

    int i = 0;
    for (const auto &a : contour.data[which_part])
    {
        vec3 pot_p = offset_p(a, w);
        if (inside(pot_p) && !detect_intersection(pot_p, contour, w))
        {
            result.push_back(pot_p);
        }

        i++;
    }
    return result;
}

vec3 inner_shell_generator::offset_p(const vec3 &p, double w) const
{
    vec2 gradient(fn_grad(p));
    gradient.Normalize();
    gradient = gradient * w;
    return p - vec3(gradient.x, gradient.y, 0);
}

bool inner_shell_generator::detect_intersection(const vec3 &of, polylines &contour, double w) const
{
    // const double danger_zone = w * 0.9;
    const double danger_zone = w * 0.99;

    polylines::iterator it = contour.begin();
    while (it.has_next())
    {
        if ((of - (*(it.get_data()))).length() < danger_zone)
        {
            return true;
        }
    }

    return false;
}
