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

weighted_polylines::weighted_polylines(const polylines &p)
{
    auto weighed = [&](vec3 p) -> std::pair<vec3, double>
    {
        return std::pair<vec3, double>{p, 11};
    };

    for (int i = 0; i < p.data.size(); i++)
    {
        data.push_back(std::vector<std::pair<vec3, double>>());
        for (int j = 0; j < p.data[i].size(); j++)
        {
            data[i].push_back(weighed(p.data[i][j]));
        }
    }
}
