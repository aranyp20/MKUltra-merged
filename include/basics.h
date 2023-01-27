#pragma once

#include <mymath.h>




struct section{

    vec2 p1,p2;

    section(const vec2& _p1, const vec2& _p2) : p1(_p1), p2(_p2){}

    inline double length() const {return (p2-p1).length();}
};