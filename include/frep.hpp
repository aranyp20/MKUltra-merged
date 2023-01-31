#pragma once

#include <iostream>
#include <functional>
#include <FunctionCreator.h>

// template <typename T>
// T fn(const T& x, const T& y, double h)  {return T(8)*(pow(x,4)+pow(y,4)+pow(T(h),4))-T(8)*(pow(x,2)+pow(y,2)+pow(T(h),2)) +T(3);}

inline double fn_sima(double x, double y, double h) { return FunctionCreator::Create(vec3(x, y, h)); }
inline interval fn_inter(const interval &i1, const interval &i2, double h) { return FunctionCreator::Create_interval(i1, i2, h); }
inline bool inside(const vec3 &p)
{
    return fn_sima(p.x, p.y, p.z) < 0;
}
/*
template <typename T>
T kernel(const T& x1, const T& y1, const T& x2, const T& y2)
{

    T m_distance = distance({x1,y1},{x2,y2});

    return m_distance;
}
*/

inline vec3 fn_grad(const vec3 &p)
{

    return FunctionCreator::Create_grad(p);
    // return vec3(32*pow(p.x,3) - 16 * p.x, 32*pow(p.y,3) - 16 * p.y, 32*pow(p.z,3) - 16 * p.z);
}
