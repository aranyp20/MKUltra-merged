#pragma once

#include <functional>
#include <memory>

#include "outer_shell_generator.h"
#include "inner_shell_generator.h"
#include "infill_generator.h"
#include "support_structure_generator.h"
#include "printable_object.h"

class slicer
{
    std::shared_ptr<frep_object> cutable_obj;
    bounding_box my_bounding_box;

    outer_shell_generator outer_generator;
    inner_shell_generator inner_generator;
    infill_generator inf_generator;
    support_structure_generator support_generator;

    sliced_object create_test_slices(unsigned int inner_shell_count, double inner_shell_distance) const;

    sliced_object::layer_data slice(double h_per_max, unsigned int inner_shell_count, double inner_shell_distance, bool slice_as_support) const;

public:
    slicer(std::shared_ptr<frep_object> _cutable_obj);

    sliced_object create_slices(unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance, std::function<void(int)>, bool slice_as_support = false) const;
    sliced_object generate_support(const sliced_object &base_sliced, const double distance_between, unsigned int level_count, unsigned int inner_shell_count, double inner_shell_distance, std::function<void(int)> cb) const;
};