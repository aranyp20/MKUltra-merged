#include <basics.h>


section_table::section_table(const std::vector<std::vector<section>>& _data) : data(_data)
{

}


section_table::iterator section_table::begin()
{
    return section_table::iterator(data);
}



bool section_table::iterator::has_next()
{
    if(data.empty() || (it_outer == data.size()-1 && it_inner == data[data.size()-1].size()-1))return false;

    if(it_inner == data[data.size()-1].size()-1){
        it_outer++;
        it_inner = 0;
    }else{
        it_inner++;
    }

    return true;
}

section* section_table::iterator::get_section() const
{
    if(data.empty())return NULL;

    return &(data[it_outer][it_inner]);
}


section_table::iterator::iterator(std::vector<std::vector<section>>& _data) : data(_data)
{

}