
#include <bits/stdc++.h>
#include <slicer.h>

template <typename T>
T fn(const T& x, const T& y, double h)  {return T(8)*(pow(x,4)+pow(y,4)+pow(T(h),4))-T(8)*(pow(x,2)+pow(y,2)+pow(T(h),2)) +T(3);}



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


bool inside(const vec3& p)
{
    return fn(p.x,p.y,p.z)<0;
}

/*
slicer::slicer(std::function<interval(const interval&, const interval&, double h)> f_interval) : frep_interval(f_interval)
{
}
*/
slicer::slicer()
{

}


slicer::square::square(const vec2& _start, double _size): start(_start), size(_size)
{

}

void slicer::print(const std::vector<section>& v) const
{
    std::string outputfile("result.obj");
    std::ofstream writer(outputfile);

    

    for(const auto& a : v){
        writer<<"v "<<a.p1.x<<" "<<a.p1.y<<" "<<0<<std::endl;
        writer<<"v "<<a.p2.x<<" "<<a.p2.y<<" "<<0<<std::endl;  
    }
    for(int i=0;i<v.size();i++){
        writer<<"l "<<i*2 +1<<" "<<i*2+2<<std::endl;
    }

}


void slicer::slice(double h, unsigned int resolution) const
{
    std::vector<square> leaves = rejection_testing(square(vec2(-1,-1),2),h,resolution);
    generate_contour(leaves,h);
    //print(generate_contour(leaves,h));
    
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
    //return frep_interval(interval(s.start.x,s.start.x+s.size),interval(s.start.y,s.start.y+s.size),h).contains(0);
    return fn(interval(s.start.x,s.start.x+s.size),interval(s.start.y,s.start.y+s.size),h).contains(0);
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

std::vector<bool> slicer::evaluate_verts(const square& s, double h) const
{
    std::vector<bool> result;

    for(int i=0;i<4;i++){
        vec2 tv = s.get_vert(i);
        result.push_back(inside(vec3(tv.x,tv.y,h)));
    }

    return result;
}


std::vector<slicer::id_section> slicer::generate_contour(const std::vector<square>& unrejecteds, double h) const
{
    std::vector<id_section> polyline;

    section_indexer section_indexer;

    for(const auto& a : unrejecteds){
        std::vector<bool> evaluated_verts = evaluate_verts(a,h);
        

        std::vector<std::pair<unsigned int,unsigned int>> ids = LUT::find_intersectables(evaluated_verts);
        std::vector<std::pair<section,section>> cutter_sections;
        std::vector<std::pair<unsigned int, unsigned int>> section_indexes;
        for(const auto& b : ids){
            cutter_sections.push_back({a.get_section(b.first),a.get_section(b.second)});
            section_indexes.push_back({section_indexer.find_add_section(a.get_section(b.first)),section_indexer.find_add_section(a.get_section(b.second))});
        }
        std::vector<id_section> ps;
        for(int i=0;i<cutter_sections.size();i++){
            id_section temp = id_section(section(calc_surfacepoint(cutter_sections[i].first,h),calc_surfacepoint(cutter_sections[i].second,h)),section_indexes[i].first,section_indexes[i].second);
            ps.push_back(temp);
        }
        polyline.insert(polyline.end(),ps.begin(),ps.end());
    }

    return polyline;
}

int counter =0;
unsigned int slicer::section_indexer::find_add_section(const section& s)
{
    for(int i=0;i<found.size();i++)
    {
        if(same_section(found[i],s)){
            return i;
        }
    }

    found.push_back(s);
    return found.size()-1;
}

bool slicer::section_indexer::same_section(const section& s1, const section& s2) const
{
    double close_enough = s1.length()/10;

    return (((s1.p1-s2.p1).length()<close_enough&&(s1.p2-s2.p2).length()<close_enough)||((s1.p2-s2.p1).length()<close_enough&&(s1.p1-s2.p2).length()<close_enough));
}

slicer::id_section::id_section(const section& s, unsigned int _start_id, unsigned int _end_id) : me(s), start_id(_start_id), end_id(_end_id)
{

}

std::vector<std::vector<section>> slicer::organise_sections(const std::vector<id_section>& unorganised) const
{
    std::vector<std::vector<section>> result;




    return result;
}



int main()
{

    slicer slicer;

    slicer.slice(0,5);

    return 0;
}