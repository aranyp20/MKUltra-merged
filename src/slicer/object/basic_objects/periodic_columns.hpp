#pragma once

#include "frep.hpp"

class periodic_columns : public frep_object
{
    double period = 0.2f;
    double radius = 0.05f;

    template <typename T>
    T put_in_four(const T &v) const
    {
        T res = v;
        double sign = -1.0;
        if (res < T(0))
            sign = 1.0;

        // ha lebegopont miatt kicsuszik akkor sokaig megy
        while (res < -T(period) / 2 || res > T(period) / 2)
        {
            res = res + T(sign) * T(period);
        }

        return res;
    }

    template <typename T, typename H, typename ARRAY = vec3_t<T>>
    T common_fn(const T &x, const T &y, const H &h) const
    {
        ARRAY p(put_in_four(x), put_in_four(y), T(h));

        ARRAY c1(T(-period / 2), T(-period / 2), T(h));
        ARRAY c2(T(period / 2), T(-period / 2), T(h));
        ARRAY c3(T(period / 2), T(period / 2), T(h));
        ARRAY c4(T(-period / 2), T(period / 2), T(h));

        return (std::min({(p - c1).length(), (p - c2).length(), (p - c3).length(), (p - c4).length()}) - T(radius));
    }

public:
    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return interval(-radius, sqrt(2) * period);
    }
    double fn(const vec3 &p) const override
    {
        return common_fn<double, double, vec3>(p.x, p.y, p.z);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return dnum(0);
    }
};
