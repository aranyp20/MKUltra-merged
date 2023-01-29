#include <inner_shell_generator.h>


polylines inner_shell_generator::generate_one(const polylines& contour, double w, double h) const
{
    polylines result;


    for(int i=0;i<contour.data.size();i++){
        result.data.push_back(generate_one_part(contour,i,w,h));
    }

    return result;
}

std::vector<vec2> inner_shell_generator::generate_one_part(const polylines& contour,unsigned int which_part, double w, double h) const
{
    std::vector<vec2> result;

    for(const auto& a : contour.data[which_part]){
        result.push_back(offset_p(a,w,h));
    }

    return result;
}

vec2 inner_shell_generator::offset_p(const vec2& p, double w, double h) const
{
    vec2 gradient(fn_grad(vec3(p.x,p.y,h)));
    gradient.Normalize();
    return p - (gradient * w);
}

bool inner_shell_generator::detect_intersection(const vec2& of, const polylines& contour, double w) const
{
    
}

