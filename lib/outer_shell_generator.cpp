#include <outer_shell_generator.h>



std::map<std::vector<bool>,std::vector<std::pair<unsigned int, unsigned int>>> outer_shell_generator::LUT::table = {
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

std::vector<std::pair<unsigned int,unsigned int>> outer_shell_generator::LUT::find_intersectables(const std::vector<bool>& s)
{
    return table[s];
}

section outer_shell_generator::square::get_section(unsigned int side_index) const
{

    switch(side_index){
        case 0: return section(get_vert(0),get_vert(1)); break;
        case 1: return section(get_vert(1),get_vert(2)); break;
        case 2: return section(get_vert(2),get_vert(3)); break;
        case 3: return section(get_vert(3),get_vert(0)); break;
        default: throw std::exception(); break;
    }
}

vec2 outer_shell_generator::square::get_vert(unsigned int vert_index) const
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

outer_shell_generator::outer_shell_generator()
{

}


outer_shell_generator::square::square(const vec2& _start, double _size): start(_start), size(_size)
{

}




polylines outer_shell_generator::generate(double h, unsigned int resolution) const
{
    std::vector<square> leaves = rejection_testing(square(vec2(-1.1,-1.1),2.2),h,resolution);
    return organiser.organise_sections(generate_contour(leaves,h),h);
    
}

std::vector<outer_shell_generator::square> outer_shell_generator::rejection_testing(const outer_shell_generator::square& s,double h,unsigned int resolution) const
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


bool outer_shell_generator::rejection_test(const square& s, double h) const
{
    return fn(interval(s.start.x,s.start.x+s.size),interval(s.start.y,s.start.y+s.size),h).contains(0);
}


std::vector<outer_shell_generator::square> outer_shell_generator::square::breakup(unsigned int count) const
{
    std::vector<outer_shell_generator::square> result;
    double lit_size = size/count;

    for(int i = 0;i<count;i++){
        for(int j=0;j<count;j++){
            result.push_back(square(vec2(start.x+lit_size*i,start.y+lit_size*j),lit_size));
        }
    }

    return result;
}


vec2 outer_shell_generator::calc_surfacepoint(const section& se, double h) const
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

std::vector<bool> outer_shell_generator::evaluate_verts(const square& s, double h) const
{
    std::vector<bool> result;

    for(int i=0;i<4;i++){
        vec2 tv = s.get_vert(i);
        result.push_back(inside(vec3(tv.x,tv.y,h)));
    }

    return result;
}


std::vector<id_section> outer_shell_generator::generate_contour(const std::vector<square>& unrejecteds, double h) const
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
            section_indexes.push_back({section_indexer.PUT_section(a.get_section(b.first)),section_indexer.PUT_section(a.get_section(b.second))});
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

unsigned int outer_shell_generator::section_indexer::PUT_section(const section& s)
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

bool outer_shell_generator::section_indexer::same_section(const section& s1, const section& s2) const
{
    double close_enough = s1.length()/10;

    return (((s1.p1-s2.p1).length()<close_enough&&(s1.p2-s2.p2).length()<close_enough)||((s1.p2-s2.p1).length()<close_enough&&(s1.p1-s2.p2).length()<close_enough));
}

id_section::id_section(const section& s, unsigned int _id_a, unsigned int _id_b) : me(s), id_a(_id_a), id_b(_id_b)
{

}

polylines section_organiser::organise_sections(const std::vector<id_section>& _unorganised, double h) const
{
    polylines result;

    std::vector<id_section> unorganised(_unorganised);

    while(!unorganised.empty()){
        id_section string_start = unorganised.back();
        unorganised.pop_back();
        
        std::vector<id_section> current_string;
        current_string.push_back(string_start);

        find_attachment(unorganised,current_string);

        std::vector<section> tmp;
        for(const auto& a : current_string){
            tmp.push_back(a.me);
        }
        result.data.push_back(serialize(tmp,h));

    }


    return result;
}

void section_organiser::find_attachment(std::vector<id_section>& from,std::vector<id_section>& current_string) const
{

    while(try_attach(from,current_string, true));
    if(try_join_ends(current_string))return;
    while(try_attach(from,current_string, false));

}

bool section_organiser::try_attach(std::vector<id_section>& from,std::vector<id_section>& current_string, bool to_end) const
{
    for(int i=0;i<from.size();i++){
        if(to_end){
            if(current_string.back().id_b==from[i].id_a){
                attach(from,i,current_string,to_end);
                return true;
            }else if(current_string.back().id_b==from[i].id_b){
                from[i].swap();
                attach(from,i,current_string,to_end);
                return true;
            }
        }else{
            if(current_string[0].id_a==from[i].id_b){
                attach(from,i,current_string,to_end);
                return true;
            }else if(current_string.back().id_b==from[i].id_b){
                from[i].swap();
                attach(from,i,current_string,to_end);
                return true;
            }
        }
    }
    return false;
}

void section_organiser::attach(std::vector<id_section>& from,unsigned int what,std::vector<id_section>& current_string, bool to_end) const
{
    if(to_end){
        current_string.push_back(from[what]);
        from.erase(from.begin() + what);
    }else{
 
        current_string.insert(current_string.begin(),from[what]);
        from.erase(from.begin() + what);
    }
}

bool section_organiser::try_join_ends(std::vector<id_section>& v) const
{
    return v.back().id_b == v[0].id_a;
}


void id_section::swap()
{
    std::swap(id_a, id_b);
    me.swap_sides();
}


