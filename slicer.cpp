#include "slicer.h"

std::map<std::vector<bool>,std::vector<std::pair<unsigned int, unsigned int>>> slicer::LUT::table = {
    {{false,false,false,false},{}},
    {{false,false,false,true},{{2,3}}},
    {{false,false,true,false},{{1,2}}},
    {{false,false,true,true},{{1,3}}},
    {{false,true,false,false},{{0,1}}},
    {{false,true,false,true},{{0,3},{1,2}}},
    {{false,true,true,false},{{0,2}}},
    {{false,true,true,true},{{0,3}}},
    {{true,false,false,false},{{0,3}}},
    {{true,false,false,true},{{0,2}}},
    {{true,false,true,false},{{0,1},{2,3}}},
    {{true,false,true,true},{{0,1}}},
    {{true,true,false,false},{{1,3}}},
    {{true,true,false,true},{{1,2}}},
    {{true,true,true,false},{{2,3}}},
    {{true,true,true,true},{{}}}
};

std::vector<std::pair<unsigned int,unsigned int>> slicer::LUT::find_intersectables(const std::vector<bool>& s)
{
    return table[s];
}

section slicer::square::get_section(unsigned int side_index) const
{

    switch(side_index){
        case 0: return section(get_vert(0),get_vert(1)); break;
        case 1: return section(get_vert(1),get_vert(2)); break;
        case 2: return section(get_vert(2),get_vert(3)); break;
        case 3: return section(get_vert(3),get_vert(0)); break;
        default: throw std::exception(); break;
    }
}

vec2 slicer::square::get_vert(unsigned int vert_index) const
{
    switch(vert_index){
        case 0: return vec2(start.x,start.y); break;
        case 1: return vec2(start.x+size,start.y); break;
        case 2: return vec2(start.x+size,start.y+size); break;
        case 3: return vec2(start.x,start.y+size); break;
        default: throw std::exception(); break;
    }
}


double a_fun(const vec3& p)
{
    return p.x*p.x + p.y*p.y - 1;
}
bool inside(const vec3& p)
{
    return a_fun(p)<=0;
}

interval t_fun(const interval& X, const interval& Y, double h)
{
    return X*X + Y*Y - interval(1,1); 
}



slicer::slicer(std::function<interval(const interval&, const interval&, double h)> f_interval) : frep_interval(f_interval)
{

}

slicer::square::square(vec2 _start, double _size): start(_start), size(_size)
{

}


void slicer::slice(double h, unsigned int resolution) const
{
    std::vector<square> leaves = rejection_testing(square(vec2(-1,-1),2),h,resolution);

    
}

std::vector<slicer::square> slicer::rejection_testing(const slicer::square& s,double h,unsigned int resolution) const
{
    resolution -= 1;

    std::vector<square> brokeup = s.breakup(3);
    std::vector<square> leaf_nodes;

    for(int i=0;i<brokeup.size();i++){
        if(rejection_test(brokeup[i],h)){
            if(resolution == 0){
                leaf_nodes.push_back(brokeup[i]);
            }else{
                std::vector<square> childs_leaves = rejection_testing(brokeup[i],h,resolution);
                leaf_nodes.insert(leaf_nodes.end(),childs_leaves.begin(),childs_leaves.end());
            }
        }
    }

    return leaf_nodes;
}


bool slicer::rejection_test(const square& s, double h) const
{
    return frep_interval(interval(s.start.x,s.start.x+s.size),interval(s.start.y,s.start.y+s.size),h).contains(0);
}


std::vector<slicer::square> slicer::square::breakup(unsigned int count) const
{
    std::vector<slicer::square> result;
    double lit_size = size/count;

    for(int i = 0;i<count;i++){
        for(int j=0;j<count;j++){
            result.push_back(square(vec2(start.x+lit_size*i,start.y+lit_size*j),lit_size));
        }
    }

    return result;
}




std::vector<section> slicer::generate_contour(const std::vector<square>& unrejecteds, double h) const
{
    std::vector<section> polyline;

    for(const auto& a : unrejecteds){
        std::vector<bool> evaluated_verts;
        for(int i=0;i<4;i++){
            vec2 tv = a.get_vert(i);
            evaluated_verts.push_back(inside(vec3(tv.x,tv.y,h)));
        }

        std::vector<std::pair<unsigned int,unsigned int>> ids = LUT::find_intersectables(evaluated_verts);
        std::vector<std::pair<section,section>> ss;
        std::vector<std::pair<vec2,vec2>> ps;
    }
}





int main()
{

    slicer slicer(t_fun);

    //slicer.slice(0,3);

    return 0;
}