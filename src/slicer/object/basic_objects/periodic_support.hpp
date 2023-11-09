#pragma once

#include <map>
#include "frep.hpp"
#include "periodic_columns.hpp"




class periodic_support : public frep_object
{
    std::shared_ptr<frep_object> surface;

    struct roof : public frep_object
    {
        std::map<vec2,double> height_map;

        void calc_height(const vec2& p) const {
            const double step=0.02;
            for(double h = 1.1; h > -1.1; h -= step){
                if(surface->inside(vec3(p.x,p.y,h))){
                    height_map.insert(p,h);
                    return;
                }
            }
            height_map.insert(p,-999);

        }

        roof(float period)
        {
            for(double x = 0; x <1.1; x +=period){
                for(double y = 0; y <1.1; y += period){

                }
            }
        }
    };


    periodic_columns cols;

public:

    periodic_support():cols(0.4f,0.03f){}

   interval fn(const interval &X, const interval &Y, double h) const override
    {
        return cols.fn(X,Y,h);
    }
    double fn(const vec3 &p) const override
    {
        return cols.fn(p);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return cols.fn(X,Y,h);
    }


};