#pragma once

#include <iostream>

struct dnum
{

    double val, der_val;

    dnum(double _val, double _der_val = 0);

    dnum operator+(dnum d) const;
    dnum operator-(dnum d) const;
    dnum operator*(dnum d) const;
    dnum operator/(dnum d) const;
};

dnum pow(const dnum &d, double n);

std::ostream &operator<<(std::ostream &os, const dnum &d);