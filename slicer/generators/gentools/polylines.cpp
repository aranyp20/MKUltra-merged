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
