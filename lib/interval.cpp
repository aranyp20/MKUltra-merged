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


interval operator+ (const interval& lhs, const interval& rhs)
{
    return interval(lhs.start+rhs.start,lhs.end+rhs.end);
}

interval operator- (const interval& lhs, const interval& rhs)
{
    return interval(lhs.start-rhs.end,lhs.end-rhs.start);
}

interval operator* (const interval& lhs, const interval& rhs)
{
    return interval(std::min({lhs.start*rhs.start,lhs.start*rhs.end,lhs.end*rhs.start,lhs.end*rhs.end}),std::max({lhs.start*rhs.start,lhs.start*rhs.end,lhs.end*rhs.start,lhs.end*rhs.end}));
}

interval operator/ (const interval& lhs, const interval& rhs)
{
    if(rhs.start==0 || rhs.end == 0) throw std::exception();

    return interval(std::min({lhs.start/rhs.start,lhs.start/rhs.end,lhs.end/rhs.start,lhs.end/rhs.end}),std::max({lhs.start/rhs.start,lhs.start/rhs.end,lhs.end/rhs.start,lhs.end/rhs.end}));
}

interval pow(const interval& inter, unsigned int power)
{
    interval tmp = inter;
    for(int i=0;i<power-1;i++){
        tmp = tmp*inter;
    }
    
    return tmp;
}
