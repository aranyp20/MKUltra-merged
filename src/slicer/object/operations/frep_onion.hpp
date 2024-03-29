#pragma once

#include "frep.hpp"

class frep_onion : public frep_object
{
    const frep_object &parent1;
    const frep_object &parent2;

    template <typename T>
    T onion(const T &v1, const T &v2) const
    {
        return T(0.5) * (v1 + v2 - sqrt(std::max(T(0), v1 * v1 + v2 * v2 - T(2) * v1 * v2)));
    }

public:
    frep_onion(const frep_object &_parent1, const frep_object &_parent2) : parent1(_parent1), parent2(_parent2) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return onion(parent1.fn(X, Y, h), parent2.fn(X, Y, h));
    }
    double fn(const vec3 &p) const override
    {
        return onion(parent1.fn(p), parent2.fn(p));
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return onion(parent1.fn(X, Y, h), parent2.fn(X, Y, h));
    }
};