#include <slicer_module.h>




int main()
{
    outer_shell_generator outer_shell_generator;
    inner_shell_generator inner_shell_generator;

    polylines pl = inner_shell_generator.generate_one(outer_shell_generator.generate(0,5),0.05,0);
    
    pl.print();

    return 0;
}

