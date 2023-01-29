#include <inner_shell_generator.h>


polylines inner_shell_generator::generate_one(polylines& contour, double w, double h) const
{
    polylines result;


    for(int i=0;i<contour.data.size();i++){
        result.data.push_back(generate_one_part(contour,i,w,h));
    }

    return result;
}

std::vector<vec2> inner_shell_generator::generate_one_part(polylines& contour,unsigned int which_part, double w, double h) const
{
    std::vector<vec2> result;

    for(const auto& a : contour.data[which_part]){
        vec2 pot_p = offset_p(a,w,h);
        bool jo = fn(pot_p.x,pot_p.y,h)<0 && !detect_intersection(pot_p,contour,w);
        if(which_part==0)std::cout<<jo<<std::endl;
        if(jo)result.push_back(pot_p);
    }

    //if(!result.empty())result.push_back(result[0]); 

    return result;
}

vec2 inner_shell_generator::offset_p(const vec2& p, double w, double h) const
{
    vec2 gradient(fn_grad(vec3(p.x,p.y,h)));
    gradient.Normalize();
    return p - (gradient * w);
}

bool inner_shell_generator::detect_intersection(const vec2& of, polylines& contour, double w) const
{
    bool found_close = false;
    const double eps = 0.0005;

    polylines::iterator it = contour.begin();
    while(it.has_next()){
        if((of-*(it.get_data())).length()<w-eps)found_close=true;
    }

    return found_close;
}

