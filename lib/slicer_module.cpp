#include <FunctionCreator.h>
#include <slicer_module.h>




void print(const std::vector<polylines>& d)
{
    std::string outputfile("result.obj");
    std::ofstream writer(outputfile);
    
    for(int k=0;k<d.size();k++){
        //lehetne majd iteratorral
        for(int j=0;j<d[k].data.size();j++){

            for(const auto& a : d[k].data[j]){        
                writer<<"v "<<a.x<<" "<<a.y<<" "<<a.z<<std::endl;
            }
        }
    }

    unsigned int counter = 0;
    for(int k=0;k<d.size();k++){
        for(int j=0;j<d[k].data.size();j++){
            for(int i=0;i<(int)(d[k].data[j].size()) -1 ;i++){
                writer<<"l "<<counter + i +1<<" "<<counter + i +2<<std::endl;
            }
            counter += d[k].data[j].size();
        }
    }

}


int main()
{
    FunctionCreator fc;
  
    outer_shell_generator outer_shell_generator;
    inner_shell_generator inner_shell_generator;

    std::vector<polylines> all;
    polylines p_outer = outer_shell_generator.generate(0,3);
    polylines p_inner = inner_shell_generator.generate_one(p_outer,10);
    all.push_back(p_outer);
    all.push_back(p_inner);
    print(all);


    return 0;
}

/*
const iterator
bisection methodot atnezni, mert a resolution javitas gyanusan sokat javit
*/