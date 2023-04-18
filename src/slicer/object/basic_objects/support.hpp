#pragma once

#include "sphere.hpp"
#include "frep_blend.hpp"
#include "frep_substract.hpp"
#include "frep_onion.hpp"

#include "periodic_columns.hpp"

class support : public frep_object
{

    cylinder m_cylinder;

    frep_scale m_sphere_scaled;

    frep_blend m_sphere_m_cylinder_blended;

    frep_substract m_substract;

    frep_substract m_c_substract;

    frep_intersect m_c_holder;

    periodic_columns m_c_pattern;

    frep_intersect m_c_ready;

    frep_onion ready;

public:
    support(const frep_object &to) : m_sphere_scaled(to, 0.15), m_sphere_m_cylinder_blended(to, 0.8, m_cylinder, 0.71), m_substract(m_sphere_m_cylinder_blended, m_sphere_scaled), m_c_substract(m_sphere_m_cylinder_blended, to), m_c_holder(m_c_substract, m_sphere_scaled), m_c_ready(m_c_holder, m_c_pattern), ready(m_c_ready, m_substract) {}

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
