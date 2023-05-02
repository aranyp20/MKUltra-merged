#pragma once

#include <fstream>

#include "printable_object.h"

class gcode_writer
{
    std::ofstream file_writer;

    void print_polylines(const polylines &);

    // [mm/min]
    const float printing_speed = 50.0f;
    const float replace_speed = 400.0f;

public:
    gcode_writer(const std::string &output_name);

    void write_gcode(const sliced_object *);
};