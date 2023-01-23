#include <algorithm>
#include <iostream>
#include "interval.h"

interval::interval(double _start,double _end) : start(_start), end(_end)
{

}

interval operator+ (const interval& left_side, const interval& right_side)
{
    return interval(left_side.start+right_side.start,left_side.end+right_side.end);
}

interval operator- (const interval& left_side, const interval& right_side)
{
    return interval(left_side.start-right_side.end,left_side.end-right_side.start);
}

interval operator* (const interval& left_side, const interval& right_side)
{
    return interval(std::min({left_side.start*right_side.start,left_side.start*right_side.end,left_side.end*right_side.start,left_side.end*right_side.end}),std::max({left_side.start*right_side.start,left_side.start*right_side.end,left_side.end*right_side.start,left_side.end*right_side.end}));
}

interval operator/ (const interval& left_side, const interval& right_side)
{
    if(right_side.start==0 || right_side.end == 0) throw std::exception();

    return interval(std::min({left_side.start/right_side.start,left_side.start/right_side.end,left_side.end/right_side.start,left_side.end/right_side.end}),std::max({left_side.start/right_side.start,left_side.start/right_side.end,left_side.end/right_side.start,left_side.end/right_side.end}));
}

bool interval::contains(double val) const
{
    return val>start && val<end;
}


int main()
{


    return 0;
}