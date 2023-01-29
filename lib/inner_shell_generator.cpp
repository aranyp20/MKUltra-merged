#include <inner_shell_generator.h>


polylines inner_shell_generator::generate_one(polylines& contour, double w, double h) const
{
    polylines result;


    for(int i=0;i<contour.data.size();i++){
        result.data.push_back(generate_one_part(contour,i,w,h));
    }

    return result;
}

std::vector<vec3> inner_shell_generator::generate_one_part(polylines& contour,unsigned int which_part, double w, double h) const
{
    std::vector<vec3> result;

    for(const auto& a : contour.data[which_part]){
        vec3 pot_p = offset_p(a,w,h);
        bool jo = fn(pot_p.x,pot_p.y,h)<0 && !detect_intersection(pot_p,contour,w);
        if(jo)result.push_back(pot_p);
    }

    //if(!result.empty())result.push_back(result[0]); 

    return result;
}

vec3 inner_shell_generator::offset_p(const vec3& p, double w, double h) const
{
    vec2 gradient(fn_grad(vec3(p.x,p.y,h)));
    gradient.Normalize();
    gradient = gradient * w; 
    return p - vec3(gradient.x,gradient.y,0);
}

bool inner_shell_generator::detect_intersection(const vec3& of, polylines& contour, double w) const
{
    const double eps = 0.005;

    polylines::iterator it = contour.begin();
    while(it.has_next()){
        if((of-(*(it.get_data()))).length()<w-eps){
            //std::cout<<(of-*(it.get_data())).length()<<" "<<w<<std::endl;
            return true;
        }
    }
    /*
    for(int i=0;i<contour.data.size();i++){
        for(int j=0;j<contour.data[i].size();j++){
            if((of - contour.data[i][j]).length()<w-eps){
                return true;
            }
        }
    }
    */

    return false;
}

