#pragma once

#include <algorithm>

struct interval
{
    double start;
    double end;

    interval(double,double);
    explicit interval(double);

    bool contains(double) const;



};

interval i_pow(const interval& inter, unsigned int power);
interval operator+ (const interval& left_side, const interval& right_side);
interval operator- (const interval& left_side, const interval& right_side);
interval operator* (const interval& left_side, const interval& right_side);
interval operator/ (const interval& left_side, const interval& right_side);



