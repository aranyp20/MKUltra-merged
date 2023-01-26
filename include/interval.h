#pragma once


struct interval
{
    double start;
    double end;

    interval(double,double);
    explicit interval(double);

    bool contains(double) const;

};

interval pow(const interval& inter, unsigned int power);
interval operator+ (const interval& left_side, const interval& right_side);
interval operator- (const interval& left_side, const interval& right_side);
interval operator* (const interval& left_side, const interval& right_side);
interval operator/ (const interval& left_side, const interval& right_side);



