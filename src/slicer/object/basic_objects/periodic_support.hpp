#pragma once

#include <memory>
#include <map>
#include "frep.hpp"
#include "periodic_columns.hpp"
#include "frep_intersect.hpp"

class periodic_support : public frep_object
{
    std::shared_ptr<frep_object> surface;

    struct roof : public frep_object
    {

        float period;

        std::shared_ptr<frep_object> surface;

        std::map<int, double> height_map;

        int calc_square_index(const vec2 &p) const
        {
            int col_count = std::round(2.2 / period) + 1;

            int res = 0;

            double start_x = -period / 2;
            while (start_x > -1.1 + period)
            {
                start_x -= period;
            }

            double start_y = -period / 2;
            while (start_y > -1.1 + period)
            {
                start_y -= period;
            }

            for (double x = start_x; x < 1.1; x += period)
            {
                for (double y = start_y; y < 1.1; y += period)
                {
                    res++;
                    if (std::fabs(p.x - x) < period / 2 && std::fabs(p.y - y) < period / 2)
                    {
                        return res;
                    }
                }
            }

            return res;
        }

        void calc_height(const vec2 &p)
        {
            const double step = 0.02;
            std::cout << calc_square_index(p) << std::endl;
            for (double h = 1.1; h > -1.1; h -= step)
            {
                if (surface->inside(vec3(p.x, p.y, h)))
                {
                    height_map[calc_square_index(p)] = h;
                    return;
                }
            }
            height_map[calc_square_index(p)] = -9999;
        }

        roof(float _period, std::shared_ptr<frep_object> _surface) : period(_period), surface(_surface)
        {

            double start_x = -period / 2;
            while (start_x > -1.1 + period)
            {
                start_x -= period;
            }

            double start_y = -period / 2;
            while (start_y > -1.1 + period)
            {
                start_y -= period;
            }
            for (double x = start_x; x < 1.1; x += period)
            {
                for (double y = start_y; y < 1.1; y += period)
                {
                    std::cout << x << " " << y << std::endl;
                    calc_height(vec2(x, y));
                }
            }
        }

        interval fn(const interval &X, const interval &Y, double h) const override
        {
            return interval(-1000, 1000);
        }
        double fn(const vec3 &p) const override
        {
            size_t s_id = calc_square_index(vec2(p.x, p.y));

            auto it = height_map.find(s_id);
            if (it != height_map.end())
            {
                return p.z - height_map.at(s_id);
            }
            else
            {
                return 9999;
            }
        }
        dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
        {
            return dnum(1, 0);
        }
    };

    periodic_columns cols;
    roof roof;

    frep_intersect ready;

public:
#define PERIOD 0.4f

    periodic_support(std::shared_ptr<frep_object> _surface) : cols(PERIOD, 0.03f), roof(PERIOD, _surface), ready(frep_intersect(roof, cols)) {}

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return interval(-1000, 1000);
    }
    double fn(const vec3 &p) const override
    {
        return ready.fn(p);
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return dnum(1, 0);
    }
};