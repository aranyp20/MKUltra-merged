#pragma once

#include <fstream>

#include "printable_object.h"

class gcode_writer
{
    std::ofstream file_writer;

public:
    gcode_writer(const std::string &output_name);

    void write_gcode(const sliced_object *);
    void print_polylines(const polylines &);
};