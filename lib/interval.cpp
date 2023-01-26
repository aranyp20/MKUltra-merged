#include <algorithm>
#include <iostream>
#include <interval.h>


interval::interval(double _start,double _end) : start(std::min(_start,_end)), end(std::max(_start,_end))
{
   
}

interval::interval(double both) : start(both), end(both)
{

}

bool interval::contains(double val) const
{
    return val>=start && val<=end;
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

interval pow(const interval& inter, unsigned int power)
{
    interval tmp = inter;
    for(int i=0;i<power-1;i++){
        tmp = tmp*inter;
    }
    
    return tmp;
}
