#include <algorithm>
#include <iostream>
#include <interval.h>
#include <math.h>
#include <cmath>
#include <mymath.h>

#include "mymath_templ.hpp"

interval::interval(double _start, double _end) : start(std::min(_start, _end)), end(std::max(_start, _end))
{
}

interval::interval(double both) : start(both), end(both)
{
}

bool interval::contains(double val) const
{
    return val >= start && val <= end;
}

interval operator+(const interval &lhs, const interval &rhs)
{
    return interval(lhs.start + rhs.start, lhs.end + rhs.end);
}

interval operator-(const interval &lhs, const interval &rhs)
{
    return interval(lhs.start - rhs.end, lhs.end - rhs.start);
}

interval operator*(const interval &lhs, const interval &rhs)
{
    return interval(std::min({lhs.start * rhs.start, lhs.start * rhs.end, lhs.end * rhs.start, lhs.end * rhs.end}), std::max({lhs.start * rhs.start, lhs.start * rhs.end, lhs.end * rhs.start, lhs.end * rhs.end}));
}

interval operator/(const interval &lhs, const interval &rhs)
{
    if (rhs.start == 0 || rhs.end == 0)
        throw std::exception();

    return interval(std::min({lhs.start / rhs.start, lhs.start / rhs.end, lhs.end / rhs.start, lhs.end / rhs.end}), std::max({lhs.start / rhs.start, lhs.start / rhs.end, lhs.end / rhs.start, lhs.end / rhs.end}));
}

bool operator<(const interval &lhs, const interval &rhs)
{
    return false;
    return lhs.end < rhs.end;
}

interval pow(const interval &inter, unsigned int power)
{
    interval tmp = inter;
    for (int i = 0; i < power - 1; i++)
    {
        tmp = tmp * inter;
    }

    return tmp;
}

interval sqrt(const interval &inter)
{
    return interval(std::min(sqrt(inter.start), sqrt(inter.end)), std::max(sqrt(inter.start), sqrt(inter.end)));
}

double interval::length() const
{
    return 0;
}

interval sin(const interval &inter)
{
    const unsigned int pontossag = 10;

    double y = (inter.start + inter.end) / 2;
    interval res = interval(sin(y));

    for (int i = 1; i <= pontossag; i++)
    {
        res = res + interval((1 / factorial(i)) * sin_d(y, i)) * pow((inter - interval(y)), i);
    }

    return res;
}

interval cos(const interval &inter)
{
    const unsigned int pontossag = 10;

    double y = (inter.start + inter.end) / 2;
    interval res = interval(cos(y));

    for (int i = 1; i <= pontossag; i++)
    {
        res = res + interval((1 / factorial(i)) * cos_d(y, i)) * pow((inter - interval(y)), i);
    }

    return res;
}

interval std::max(const interval &i1, const interval &i2)
{
    return interval(std::max(i1.start, i2.start), std::max(i1.end, i2.end));
}

std::ostream &operator<<(std::ostream &os, const interval &i)
{
    os << "[" << i.start << ", " << i.end << "]";
    return os;
}

template <>
interval vec3_t<interval>::length() const
{
    return sqrt(std::max(interval(0), dot(*this, *this)));
};
