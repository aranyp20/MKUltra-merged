#include <polylines.h>

void polylines::add_together(const polylines &other)
{
    for (const auto &a : other.data)
    {
        data.push_back(a);
    }
}
