#include <polylines.h>

void polylines::add_together(const polylines &other)
{
    for (const auto &a : other.data)
    {
        data.push_back(a);
    }
}

void polylines::eat(const std::vector<vec3> &what)
{
    data.push_back(what);
}

weighted_polylines::weighted_polylines(const polylines &p, std::function<double(const vec3 &, frep_object *)> weigher_function, frep_object *surf)
{
    for (int i = 0; i < p.data.size(); i++)
    {
        data.push_back(std::vector<std::pair<vec3, double>>());
        for (int j = 0; j < p.data[i].size(); j++)
        {
            data[i].push_back(std::pair<vec3, double>{p.data[i][j], weigher_function(p.data[i][j], surf)});
        }
    }
}
