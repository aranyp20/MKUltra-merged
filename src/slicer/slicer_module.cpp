#include "FunctionCreator.h"
#include "slicer_module.h"
#include "settings.h"
#include "fdm_obj.hpp"
#include "gyroid.hpp"

#define SQUARE_DEPTH 5

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

slicer::slicer(std::shared_ptr<frep_object> _cutable_obj) : cutable_obj(_cutable_obj), my_bounding_box(_cutable_obj->get_prefered_box()), outer_generator(outer_shell_generator(_cutable_obj)), inner_generator(inner_shell_generator(cutable_obj)), inf_generator(infill_generator(cutable_obj)), support_generator(support_structure_generator(_cutable_obj))
{
}

sliced_object::layer_data slicer::slice_fdm(double h_per_max, double offset) const
{
    const double h = my_bounding_box.floor.first.z + h_per_max * my_bounding_box.height;

    std::shared_ptr<gyroid> infill_pat = std::make_shared<gyroid>();
    std::shared_ptr<fdm_obj> cuttable_fdm = std::make_shared<fdm_obj>(cutable_obj, infill_pat, offset);

    outer_shell_generator outer_2(cuttable_fdm);
    infill_generator infill_2(cuttable_fdm);

    polylines inner;
    polylines outer = outer_2.generate(std::pair<vec2, double>(my_bounding_box.floor), h, SQUARE_DEPTH);
    polylines infill = infill_2.generate(std::pair<vec2, double>(my_bounding_box.floor), h, 1, settings::fdm_fullfill_distance, 0);

    return sliced_object::layer_data(outer, inner, infill, my_bounding_box, false);
}

sliced_object::layer_data slicer::slice(double h_per_max, unsigned int inner_shell_count, double inner_shell_distance, bool slice_as_support) const
{
    const double h = my_bounding_box.floor.first.z + h_per_max * my_bounding_box.height;

    polylines inner;
    polylines infill;
    polylines outer = outer_generator.generate(std::pair<vec2, double>(my_bounding_box.floor), h, SQUARE_DEPTH);
    infill.add_together(inf_generator.generate(std::pair<vec2, double>(my_bounding_box.floor), h, M_PI / settings::infill_number_rot, settings::infill_space_between, inner_shell_count * inner_shell_distance));
    for (int i = 1; i <= inner_shell_count; i++)
    {
        polylines inner_l = inner_generator.generate_one(outer, inner_shell_distance * i);
        inner.add_together(inner_l);
    }
    return sliced_object::layer_data(outer, inner, infill, my_bounding_box, slice_as_support);
}

sliced_object slicer::create_test_slices(unsigned int inner_shell_count, double inner_shell_distance) const
{
    std::vector<sliced_object::layer_data> result;

    for (int i = 0; i < 3; i++)
    {
        sliced_object::layer_data level = slice(0.45 + i * 0.05, inner_shell_count, inner_shell_distance, false);
        result.push_back(level);
    }

    return sliced_object(result, my_bounding_box);
}

sliced_object slicer::create_slices(unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance, std::function<void(int)> cb, bool slice_as_support) const
{
    std::vector<sliced_object::layer_data> result;

    for (int i = 0; i < level_count; i++)
    {
        sliced_object::layer_data level;
        if (settings::as_fdm)
        {
            level = slice_fdm(i / (double)level_count, inner_shell_distance);
        }
        else
        {
            level = slice(i / (double)level_count /*floor to one lvl below ceiling*/, inner_shell_count, inner_shell_distance, slice_as_support);
        }
        result.push_back(level);
        cb((i + 1) * 100 / level_count);
    }
    sliced_object obj(result, my_bounding_box);

    return obj;
}

sliced_object slicer::generate_support(const sliced_object &base_sliced, const double distance_between, unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance, std::function<void(int)> cb) const
{
    return support_generator.generate_to(base_sliced, distance_between, level_count, inner_shell_count, inner_shell_distance, cb);
}
