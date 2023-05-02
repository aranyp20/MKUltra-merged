#include "gcode_writer.h"

gcode_writer::gcode_writer(const std::string &output_name) : file_writer(output_name)
{
}

void gcode_writer::print_polylines(const polylines &p)
{
    for (unsigned int i = 0; i < p.data.size(); i++)
    {
        for (unsigned int j = 0; j < p.data[i].size(); j++)
        {
            if (j == 0)
            {
                file_writer << "G00 X" << p.data[i][j].x << " Y" << p.data[i][j].y << std::endl;
            }
            else
            {
                file_writer << "G01 X" << p.data[i][j].x << " Y" << p.data[i][j].y << std::endl;
            }
        }
    }
}

void gcode_writer::write_gcode(const sliced_object *obj)
{
    file_writer << "%" << std::endl;
    for (unsigned int i = 0; i < obj->get_slice_count(); i++)
    {
        print_polylines(obj->get_poly_level(i));
    }
    file_writer << "%" << std::endl;
}
