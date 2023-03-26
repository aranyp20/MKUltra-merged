#pragma once
#include <iostream>

#include "mymath_templ.hpp"

struct interval
{
    double start;
    double end;

    interval(double, double);
    explicit interval(double);

    bool contains(double) const;

    double length() const;
};

interval pow(const interval &inter, unsigned int power);
interval operator+(const interval &lhs, const interval &rhs);
interval operator-(const interval &lhs, const interval &rhs);
interval operator*(const interval &lhs, const interval &rhs);
interval operator/(const interval &lhs, const interval &rhs);

interval sqrt(const interval &inter);
interval sin(const interval &inter);
interval cos(const interval &inter);

namespace std
{
    interval max(const interval &, const interval &);
}

std::ostream &operator<<(std::ostream &, const interval &);

/*
struct vec3_interval : public vec3_t<interval>
{
    vec3_interval(const vec3_t<interval> &p) : vec3_t(p) {}

    template <typename T>
    inline vec3_t<T> length() const override { return sqrt(x * x + y * y + z * z); }

};
*/