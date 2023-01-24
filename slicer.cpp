#include <bits/stdc++.h>
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
    {{true,true,true,true},{}}
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

/*
double a_fun(const vec3& p)
{
    return p.x*p.x + p.y*p.y + p.z*p.z - 1;
}

interval t_fun(const interval& X, const interval& Y, double h)
{
    return X*X + Y*Y + interval(h-0.01,h+0.01)*interval(h-0.01,h+0.01) - interval(1,1); 
}
*/
double a_fun(const vec3& p)
{
    return 8*(pow(p.x,4)+pow(p.y,4)+pow(p.z,4))-8*(pow(p.x,2)+pow(p.y,2)+pow(p.z,2)) +3;
}


interval t_fun(const interval& X, const interval& Y, double h)
{
    interval tm(h,h);
    return interval(8,8)*(X*X*X*X+Y*Y*Y*Y+tm*tm*tm*tm)-interval(8,8)*(X*X+Y*Y+tm*tm) +interval(3,3); 
}

bool inside(const vec3& p)
{
    return a_fun(p)<0;
}

slicer::slicer(std::function<interval(const interval&, const interval&, double h)> f_interval) : frep_interval(f_interval)
{

}

slicer::square::square(vec2 _start, double _size): start(_start), size(_size)
{

}

void slicer::print(const std::vector<section>& v) const
{
    std::string outputfile("result.vtk");
    std::ofstream writer(outputfile);

    writer<<"# vtk DataFile Version 2.0"<<std::endl;
    writer<<"Alma2"<<std::endl;
    writer<<"ASCII"<<std::endl;
    writer<<"DATASET POLYDATA"<<std::endl;
    writer<<"POINTS "<<v.size()<<" float"<<std::endl;

    for(const auto& a : v){
        writer<<a.p2.x<<" "<<a.p2.y<<" "<<0<<std::endl;
    }

}


void slicer::slice(double h, unsigned int resolution) const
{
    std::vector<square> leaves = rejection_testing(square(vec2(-1,-1),2),h,resolution);
    print(generate_contour(leaves,h));
    
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


vec2 slicer::calc_surfacepoint(const section& se, double h) const
{
    vec2 ip,op;
    if(inside(vec3(se.p1.x,se.p1.y,h))&&!inside(vec3(se.p2.x,se.p2.y,h))){
        ip = vec2(se.p1);
        op = vec2(se.p2);
    }else if(!inside(vec3(se.p1.x,se.p1.y,h))&&inside(vec3(se.p2.x,se.p2.y,h))){
        ip = vec2(se.p2);
        op = vec2(se.p1);
    }else{
        //std::cout<<"2 pont ugyanott"<<std::endl;
        throw std::exception();
    }

    vec2 mid = (ip+op)/2;

    for(int i=0;i<10;i++){
        if(!inside(vec3(mid.x,mid.y,h))){
            mid = (ip+mid)/2;
            op = mid;
        }else{
            mid = (op+mid)/2;
            ip=mid;
        }
    }

    return mid;
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
        for(const auto& b : ids){
            ss.push_back({a.get_section(b.first),a.get_section(b.second)});
        }
        std::vector<section> ps;
        for(int i=0;i<ss.size();i++){
            section temp = section(calc_surfacepoint(ss[i].first,h),calc_surfacepoint(ss[i].second,h));
            if(temp.p1.x==0||temp.p2.x==0)std::cout<<ids[i].first<<" "<<ids[i].second<<std::endl;
            ps.push_back(temp);
        }
        polyline.insert(polyline.end(),ps.begin(),ps.end());
    }
    return polyline;
}





int main()
{

    slicer slicer(t_fun);

    slicer.slice(0,5);

    return 0;
}