#pragma once

#include "frep.hpp"
#include "frep_intersect.hpp"
#include "frep_invert.hpp"

class frep_substract : public frep_object
{
    const frep_object &parent1;
    const frep_object &parent2;

    // TODO: why I need this intersect??
    frep_intersect m_intersect;
    frep_invert m_invert;

    frep_intersect ready;

public:
    frep_substract(const frep_object &_parent1, const frep_object &_parent2) : parent1(_parent1), parent2(_parent2), m_intersect(parent1, parent2), m_invert(m_intersect), ready(parent1, m_invert) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return ready.fn(X, Y, h);
    }
    double fn(const vec3 &p) const override
    {
        return ready.fn(p);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return ready.fn(X, Y, h);
    }
};