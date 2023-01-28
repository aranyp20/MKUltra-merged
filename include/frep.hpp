#pragma once

#include <iostream>
#include <functional>



template <typename T>
T fn(const T& x, const T& y, double h)  {return T(8)*(pow(x,4)+pow(y,4)+pow(T(h),4))-T(8)*(pow(x,2)+pow(y,2)+pow(T(h),2)) +T(3);}

//TODO
inline vec3 fn_grad(const vec3& p) {


    return vec3(32*pow(p.x,3) - 16 * p.x, 32*pow(p.y,3) - 16 * p.y, 32*pow(p.z,3) - 16 * p.z);

}
