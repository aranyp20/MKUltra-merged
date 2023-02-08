#include <fstream>
#include "FunctionCreator.h"
#include "slicer_module.h"

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

slicer::slicer(frep_object *_cutable_obj, const bounding_box &_box) : cutable_obj(_cutable_obj), my_bounding_box(_box), outer_generator(outer_shell_generator(_cutable_obj)), inner_generator(inner_shell_generator(cutable_obj)), inf_generator(infill_generator(cutable_obj))
{
}

slicer::bounding_box::bounding_box(const vec3 &_corner, double _width, double _height) : floor(std::pair<vec3, double>{_corner, _width}), height(_height)
{
}

std::vector<polylines> slicer::slice(double h_per_max, unsigned int inner_shell_count, double inner_shell_distance) const
{
    std::vector<polylines> level;

    double h = my_bounding_box.floor.first.z + h_per_max * my_bounding_box.height;
    polylines outer = outer_generator.generate(my_bounding_box.floor, h, 5);
    level.push_back(outer);
    level.push_back(inf_generator.generate(std::pair<vec2, double>(my_bounding_box.floor), h, M_PI / 3, 0.08, 0.06));
    for (int i = 1; i <= inner_shell_count; i++)
    {
        polylines inner = inner_generator.generate_one(outer, inner_shell_distance * i);
        level.push_back(inner);
    }
    return level;
}

void slicer::create_slices(unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance) const
{
    std::vector<polylines> result;

    for (int i = 0; i < level_count; i++)
    {
        std::vector<polylines> level = slice(i / (double)level_count /*floor to one lvl below ceiling*/, inner_shell_count, inner_shell_distance);
        result.insert(result.end(), level.begin(), level.end());
    }
    print(result);
}

int main()
{

    frep_object *cutable_obj = new chmutov();
    slicer::bounding_box bb1(vec3(-1.10, -1.10, -1.10), 2.20, 2.20);
    slicer::bounding_box bb2(vec3(-110, -110, -110), 220, 220);

    slicer slicer(cutable_obj, bb1);
    slicer.create_slices(10, 2, 0.02);

    return 0;
}

/*
BUGS:

inner shell generator utolso es nulladik pontja kozott levo szakasz - megint megjelent valami szakadas, de aztan eltunik .^.

TODO:
planet hasznalni mindenhol
functioncreater createje ne legyen static

polylines eatet hasznalni mindenhol

std::vector<polylines>-nak lehet nincs sok ertelme (mondjuk arra jo h a dolgokat elkulonitsuk)
2D forgato matrix

vec2->vec3 illetve az egesz mymath.h-hoz cpp, vec2-re jo normalize-t meg ilyeneket stb stb..
squaret es planet valamennyire ossze lehetne vonni
const iterator
iterator jaitas: it->data
polylines es sectionvector vegeinek osszekotese
RBF kiertekelst optimalizalni
*/
