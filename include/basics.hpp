#pragma once

#include <vector>
#include <mymath.h>
#include <bits/stdc++.h>





struct section{

    vec2 p1,p2;

    section(const vec2& _p1, const vec2& _p2) : p1(_p1), p2(_p2){}

    inline double length() const {return (p2-p1).length();}
    inline void swap_sides() {
        std::swap(p1,p2);
    }


};


//ennek kell egy sectionstring osztaly ami az osszekotest is tarolja
inline std::vector<vec3> serialize(const std::vector<section>& v, double h){
    std::vector<vec3> result;
    for(const auto& a : v){
        result.push_back(vec3(a.p1.x,a.p1.y,h));
    }
    result.push_back(vec3(v.back().p2.x,v.back().p2.y,h));

    return result;
}



template <typename T>
struct data_table{

    std::vector<std::vector<T>> data;
    //kell egy const valtozat 
    struct iterator{
        
        std::vector<std::vector<T>>& data;


        bool untouched = true;

        bool has_next(){
            
            if(data.empty() || (it_outer == data.size()-1 && it_inner == data[data.size()-1].size()))return false;

            if(untouched){untouched = false;return true;}

            if(it_inner == data[it_outer].size()){
                it_outer++;
                it_inner = 0;
            }else{
                it_inner++;
            }

            return true;
        }

        T* get_data() const{
            if(data.empty())return NULL;

            return &(data[it_outer][it_inner]);
        }

        
    
        iterator(std::vector<std::vector<T>>& _data) : data(_data) {}

    private:
    
        unsigned int it_outer = 0;
        unsigned int it_inner = 0;


    };

    iterator begin() {return data_table::iterator(data);}


    data_table(){}
    data_table(std::vector<std::vector<T>>& _data) : data(_data) {}
};

struct polylines : public data_table<vec3>{

    void print() const {
        

    }
  
};
//innen sokat cpp-be pakolni