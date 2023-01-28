#include <slicer_module.h>
#include <bits/stdc++.h>


void print(const std::vector<std::vector<section>>& v)
{
    std::string outputfile("result.obj");
    std::ofstream writer(outputfile);
    
    for(int j=0;j<v.size();j++){

        for(const auto& a : v[j]){        
            writer<<"v "<<a.p1.x<<" "<<a.p1.y<<" "<<0<<std::endl;
            writer<<"v "<<a.p2.x<<" "<<a.p2.y<<" "<<0<<std::endl;  
        }
    }

    unsigned int counter = 0;
    for(int j=0;j<v.size();j++){
        for(int i=0;i<v[j].size();i++){
            writer<<"l "<<counter + i*2 +1<<" "<<counter + i*2+2<<std::endl;
        }
        counter += v[j].size()*2;
    }
}

int main()
{
    outer_shell_generator outer_shell_generator;
    inner_shell_generator inner_shell_generator;

    print(inner_shell_generator.generate_one(outer_shell_generator.generate(0,5),0.05,0).data);

    return 0;
}

