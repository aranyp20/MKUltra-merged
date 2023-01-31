#pragma once


struct interval
{
    double start;
    double end;

    interval(double,double);
    explicit interval(double);

    bool contains(double) const;

    double length() const;

};

interval pow(const interval& inter, unsigned int power);
interval operator+ (const interval& lhs, const interval& rhs);
interval operator- (const interval& lhs, const interval& rhs);
interval operator* (const interval& lhs, const interval& rhs);
interval operator/ (const interval& lhs, const interval& rhs);

interval sqrt(const interval& inter);


