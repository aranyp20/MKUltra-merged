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
                file_writer << "G0 X" << p.data[i][j].x << " Y" << p.data[i][j].y << std::endl;
            }
            else
            {
                file_writer << "G1 X" << p.data[i][j].x << " Y" << p.data[i][j].y << std::endl;
            }
        }
    }
}

void gcode_writer::write_gcode(const sliced_object *obj)
{
    file_writer << "; Init:\n";
    file_writer << "G28 X Y ; home horizontally\n";
    file_writer << "G28 Z ; home vertically\n";
    file_writer << "M104 S215 ;set head temp (no wait)\n";
    file_writer << "M190 S60 ;set bed temp (wait)\n";
    file_writer << "M109 S215 ;set head temp (wait)\n\n";
    file_writer << "; Layer[0]:\n";
    file_writer << "M106 S0 ;turn off fan" << std::endl;

    for (unsigned int i = 0; i < obj->get_slice_count(); i++)
    {
        file_writer << "; Layer[" << i << "]:\n";
        file_writer << "G21 ; mm\n";
        file_writer << "G92 E0 ; reset extr\n";
        file_writer << "G90 ; abs\n";
        file_writer << "G0 F300 Z" << 0.300 * (i + 1) << std::endl;
        print_polylines(obj->get_poly_level(i));
    }

    file_writer << "G0X0Y0\n";
    file_writer << "M104 S0 ;turn off head heater\n";
    file_writer << "M140 S0 ;turn off bed heater\n";
    file_writer << "M106 S0" << std::endl;
}
