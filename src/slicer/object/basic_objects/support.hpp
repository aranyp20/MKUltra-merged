#pragma once

#include "sphere.hpp"
#include "frep_blend.hpp"
#include "frep_substract.hpp"
#include "frep_onion.hpp"
#include "cylinder_section.hpp"

#include "periodic_columns.hpp"

class support : public frep_object
{

    std::shared_ptr<frep_object> pto;

    cylinder_section m_cylinder_bot;

    cylinder_section m_cylinder_top;

    frep_scale m_sphere_scaled;

    frep_blend m_sphere_m_cylinder_blended;

    frep_substract m_substract;

    frep_substract m_c_substract;

    frep_intersect m_c_holder;

    periodic_columns m_c_pattern;

    frep_intersect m_c_ready;

    frep_onion ready;

    frep_onion done;

    vec3 calc_sep_point(const vec3 &ground_point, const vec3 &hold_point) const
    {
        vec3 axis = ground_point - hold_point;
        return hold_point + axis / 4;
    }

public:
    support(std::shared_ptr<frep_object> to, const vec3 &ground_point, const vec3 &hold_point) : pto(to), m_cylinder_bot(ground_point, calc_sep_point(ground_point, hold_point)), m_cylinder_top(calc_sep_point(ground_point, hold_point), hold_point), m_sphere_scaled(*pto, 0.15), m_sphere_m_cylinder_blended(*pto, 2.1, m_cylinder_top, 0.31), m_substract(m_sphere_m_cylinder_blended, m_sphere_scaled), m_c_substract(m_sphere_m_cylinder_blended, *pto), m_c_holder(m_c_substract, m_sphere_scaled), m_c_ready(m_c_holder, m_c_pattern), ready(m_c_ready, m_substract), done(m_cylinder_bot, ready) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return done.fn(X, Y, h);
    }
    double fn(const vec3 &p) const override
    {
        return done.fn(p);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return done.fn(X, Y, h);
    }
};
