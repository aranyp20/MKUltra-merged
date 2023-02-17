#include <fstream>
#include "FunctionCreator.h"
#include "slicer_module.h"
#include "settings.h"

/*
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
*/

slicer::slicer(frep_object *_cutable_obj) : cutable_obj(_cutable_obj), my_bounding_box(_cutable_obj->get_prefered_box()), outer_generator(outer_shell_generator(_cutable_obj)), inner_generator(inner_shell_generator(cutable_obj)), inf_generator(infill_generator(cutable_obj))
{
}

polylines slicer::slice(double h_per_max, unsigned int inner_shell_count, double inner_shell_distance) const
{
    polylines level;

    double h = my_bounding_box.floor.first.z + h_per_max * my_bounding_box.height;
    polylines outer = outer_generator.generate(my_bounding_box.floor, h, 5);
    level.add_together(outer);
    level.add_together(inf_generator.generate(std::pair<vec2, double>(my_bounding_box.floor), h, M_PI / settings::infill_number_rot, settings::infill_space_between, inner_shell_count * inner_shell_distance));
    for (int i = 1; i <= inner_shell_count; i++)
    {
        polylines inner = inner_generator.generate_one(outer, inner_shell_distance * i);
        level.add_together(inner);
    }
    return level;
}

sliced_object slicer::create_slices(unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance) const
{
    std::vector<polylines> result;

    for (int i = 0; i < level_count; i++)
    {
        polylines level = slice(i / (double)level_count /*floor to one lvl below ceiling*/, inner_shell_count, inner_shell_distance);
        result.push_back(level);
    }
    // print(result);
    return sliced_object(result, std::pair<vec2, double>(my_bounding_box.floor));
}

/*
BUGS:

inner shell generator utolso es nulladik pontja kozott levo szakasz - megint megjelent valami szakadas, de aztan eltunik .^.

TODO:
slice()-nak paramobject
planet hasznalni mindenhol
functioncreater createje ne legyen static

infillnel a generatorsectionok egy atlobol csinalodjanak
Qobject::connect -hez fuggveny ami inicializa is (ami pedig jobb hogy a surface ad egy recommendationt es azzal kitolt mindent)
polylines eatet hasznalni mindenhol
settingsben ha megvaltozik valami arrol ertesitse a viewt
guiban atnevezesek(snakecase)
std::vector<polylines>-nak lehet nincs sok ertelme (mondjuk arra jo h a dolgokat elkulonitsuk)
2D forgato matrix

statusbar surface betoltesehez

vec2->vec3 illetve az egesz mymath.h-hoz cpp, vec2-re jo normalize-t meg ilyeneket stb stb..
squaret es planet valamennyire ossze lehetne vonni
const iterator
iterator jaitas: it->data
polylines es sectionvector vegeinek osszekotese
RBF kiertekelst optimalizalni
inner_fill-nel a relevant_section-oket eleg lenne egyszer kiszamolni
*/
