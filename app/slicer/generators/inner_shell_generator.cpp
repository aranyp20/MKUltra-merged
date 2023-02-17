#include <inner_shell_generator.h>

inner_shell_generator::inner_shell_generator(frep_object *_surface) : surface(_surface) {}

polylines inner_shell_generator::generate_one(polylines &contour, double w) const
{
    polylines result;

    for (int i = 0; i < contour.data.size(); i++)
    {
        result.add_together(generate_one_part(contour, i, w));
    }

    return result;
}

void inner_shell_generator::point_was_good(const vec3 &p, std::vector<vec3> &current_string, vec3 &last_pos, bool &active) const
{
    if (!active)
    {
        current_string.push_back(last_pos);
    }
    current_string.push_back(p);
    active = true;
    last_pos = p;
}

void inner_shell_generator::point_was_bad(const vec3 &p, std::vector<vec3> &current_string, vec3 &last_pos, bool &active, polylines &result) const
{
    if (active)
    {
        result.data.push_back(current_string);
        current_string.clear();
    }
    active = false;
    last_pos = p;
}

void handle_ending(std::vector<vec3> &current_string, vec3 &last_pos, bool &active, polylines &result)
{

    if (!current_string.empty())
    {
        result.data.push_back(current_string);
    }
}

void inner_shell_generator::generate_one_point(polylines &result, polylines &contour, const vec3 &from, std::vector<vec3> &current_string, vec3 &last_pos, double w, bool &active) const
{
    vec3 pot_p = offset_p(from, w);
    if (surface->inside(pot_p) && !detect_intersection(pot_p, contour, w))
    {
        point_was_good(pot_p, current_string, last_pos, active);
    }
    else if (surface->inside(pot_p))
    {
        point_was_bad(pot_p, current_string, last_pos, active, result);
    }
}

polylines inner_shell_generator::generate_one_part(polylines &contour, unsigned int which_part, double w) const
{
    polylines result;

    vec3 last_pos = offset_p(contour.data[which_part][0], w);
    std::vector<vec3> current_string;
    vec3 first_point = last_pos;
    if (surface->inside(first_point) && !detect_intersection(first_point, contour, w))
    {
        current_string.push_back(first_point);
    }
    bool active = true;
    for (int i = 1; i < contour.data[which_part].size(); i++)
    {
        generate_one_point(result, contour, contour.data[which_part][i], current_string, last_pos, w, active);
    }

    handle_ending(current_string, last_pos, active, result);

    return result;
}

vec3 inner_shell_generator::offset_p(const vec3 &p, double w) const
{
    vec2 gradient(surface->grad(p));
    gradient.Normalize();
    gradient = gradient * w;
    return p - vec3(gradient.x, gradient.y, 0);
}

bool inner_shell_generator::detect_intersection(const vec3 &of, polylines &contour, double w) const
{
    const double danger_zone = w * 0.8;

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
