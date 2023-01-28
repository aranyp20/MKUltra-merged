#include <inner_shell_generator.h>


section_table inner_shell_generator::generate_one(const section_table& contour, double w, double h) const
{
    section_table result;

    for(int i=0;i<contour.data.size();i++){
        result.data.push_back(generate_one_part(contour,i,w,h));
    }

    return result;
}

std::vector<section> inner_shell_generator::generate_one_part(const section_table& contour,unsigned int which_part, double w, double h) const
{
    std::vector<section> result;

    for(const auto& a : contour.data[which_part]){
        result.push_back(section(offset_p(a.p1,w,h),offset_p(a.p2,w,h)));
    }
    
    return result;
}

vec2 inner_shell_generator::offset_p(const vec2& p, double w, double h) const
{
    vec2 gradient(fn_grad(vec3(p.x,p.y,h)));
    gradient.Normalize();
    return p - (gradient * w);
}

bool inner_shell_generator::detect_intersection(const vec2& of, const section_table& contour, double w) const
{
    
}

