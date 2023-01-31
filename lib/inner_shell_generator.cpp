#include <inner_shell_generator.h>


polylines inner_shell_generator::generate_one(polylines& contour, double w) const
{
    polylines result;


    for(int i=0;i<contour.data.size();i++){
        result.data.push_back(generate_one_part(contour,i,w));
    }

    return result;
}

std::vector<vec3> inner_shell_generator::generate_one_part(polylines& contour,unsigned int which_part, double w) const
{
    std::vector<vec3> result;

    for(const auto& a : contour.data[which_part]){
        vec3 pot_p = offset_p(a,w);
        bool jo = fn_sima(pot_p.x,pot_p.y,pot_p.z)<0 && !detect_intersection(pot_p,contour,w);
        if(jo)result.push_back(pot_p);
    }

    //if(!result.empty())result.push_back(result[0]); 

    return result;
}

vec3 inner_shell_generator::offset_p(const vec3& p, double w) const
{
    vec2 gradient(fn_grad(p));
    gradient.Normalize();
    gradient = gradient * w; 
    return p - vec3(gradient.x,gradient.y,0);
}

bool inner_shell_generator::detect_intersection(const vec3& of, polylines& contour, double w) const
{
    const double eps = 0.005;

    polylines::iterator it = contour.begin();
    while(it.has_next()){
        if((of-(*(it.get_data()))).length()<w-eps)return true;
    
    }

    return false;
}

