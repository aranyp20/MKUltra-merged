#include "gcode_writer.h"

gcode_writer::gcode_writer(const std::string &output_name) : file_writer(output_name)
{
}

void gcode_writer::write_gcode(const sliced_object *obj)
{
}

void gcode_writer::print_polylines(const polylines &p)
{
}
