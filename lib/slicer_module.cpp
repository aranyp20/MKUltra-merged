#include <slicer_module.h>
#include <bits/stdc++.h>


void print(const std::vector<std::vector<section>>& v)
{
    std::string outputfile("result.obj");
    std::ofstream writer(outputfile);
    
    for(const auto& a : v[0])
    {        
        writer<<"v "<<a.p1.x<<" "<<a.p1.y<<" "<<0<<std::endl;
        writer<<"v "<<a.p2.x<<" "<<a.p2.y<<" "<<0<<std::endl;  
    }

    for(int i=0;i<v[0].size();i++){
        writer<<"l "<<i*2 +1<<" "<<i*2+2<<std::endl;
    }
}

int main()
{
    outer_shell_generator outer_shell_generator;

    print(outer_shell_generator.generate(0,5));

    return 0;
}