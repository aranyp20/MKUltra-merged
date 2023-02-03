#include <fstream>
#include <FunctionCreator.h>
#include <slicer_module.h>

void print(const std::vector<polylines> &d)
{
    std::cout << "Printing..." << std::endl;
    std::string outputfile("result.obj");
    std::ofstream writer(outputfile);

    for (int k = 0; k < d.size(); k++)
    {
        for (int j = 0; j < d[k].data.size(); j++)
        {

            for (const auto &a : d[k].data[j])
            {
                writer << "v " << a.x << " " << a.y << " " << a.z << std::endl;
            }
        }
    }

    unsigned int counter = 0;
    for (int k = 0; k < d.size(); k++)
    {
        for (int j = 0; j < d[k].data.size(); j++)
        {

            for (int i = 0; i < (int)(d[k].data[j].size()) - 1; i++)
            {
                writer << "l " << counter + i + 1 << " " << counter + i + 2 << std::endl;
            }
            counter += d[k].data[j].size();
        }
    }
    std::cout << "Printing ended" << std::endl;
}

int main()
{
    // FunctionCreator fc;

    frep_object *cutable_obj = new chmutov();

    outer_shell_generator outer_shell_generator(cutable_obj);
    inner_shell_generator inner_shell_generator(cutable_obj);

    std::vector<polylines> all;
    for (int i = 0; i < 10; i++)
    {
        // std::pair<vec2, double> bounding_box = {vec2(-110, -110), 220};
        std::pair<vec2, double> bounding_box = {vec2(-1.1, -1.1), 2.2};

        // const double h = -110 + i * 22;
        const double h = -1.1 + i * 0.22;

        polylines p_outer = outer_shell_generator.generate(bounding_box, h, 5);

        for (int j = 0; j < 3; j++)
        {
            // const double w = 3;
            const double w = 0.09;

            polylines p_inner = inner_shell_generator.generate_one(p_outer, w * (j + 1));
            all.push_back(p_inner);
        }

        all.push_back(p_outer);
    }
    print(all);

    return 0;
}

/*
BUGS:

inner shell generator utolso es nulladik pontja kozott levo szakasz

TODO:

const iterator
iterator jaitas: it->data
polylines es sectionvector vegeinek osszekotese
RBF kiertekelst optimalizalni
*/