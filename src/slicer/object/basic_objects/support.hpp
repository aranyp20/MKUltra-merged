#pragma once

#include "sphere.hpp"
#include "frep_blend.hpp"
#include "frep_substract.hpp"
#include "frep_onion.hpp"

class support : public frep_object
{

    sphere m_sphere;
    cylinder m_cylinder;

    frep_scale m_sphere_scaled;

    frep_blend m_sphere_m_cylinder_blended;

    frep_substract m_substract;

    frep_onion m_onion;

    frep_onion ready;

public:
    support() : m_sphere_scaled(m_sphere, 0.15), m_sphere_m_cylinder_blended(m_sphere, 0.8, m_cylinder, 0.71), m_substract(m_sphere_m_cylinder_blended, m_sphere_scaled), m_onion(m_sphere, m_substract), ready(m_onion) {}

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
