#include "gcode_writer.h"

gcode_writer::gcode_writer(const std::string &output_name) : file_writer(output_name)
{
}

float E = 0.0f;
bool F_for_G1 = true;

float to_print(float p){
    return p * 10.0f + 50.0f;
}

vec2 to_print(const vec2& v){
    return vec2(to_print(v.x), to_print(v.y));
}

void gcode_writer::print_polylines(const polylines &p)
{
    for (unsigned int i = 0; i < p.data.size(); i++)
    {
        for (unsigned int j = 0; j < p.data[i].size(); j++)
        {
            if (j == 0)
            {
                const float set_back_E = std::max(0.0f,(E-1.0f));
                file_writer << "G0 F4800 E" << set_back_E<<"\n";
                file_writer << "G0 X" << to_print(p.data[i][j].x)<< " Y" << to_print(p.data[i][j].y)<< " F10800\n";
                file_writer << "G0 F4800 E" << E << std::endl;
                F_for_G1 = true;

            }
            else
            {
                const float travel_distance = (to_print(p.data[i][j-1]) - to_print(p.data[i][j])).length();
                E += travel_distance * 0.05;
                file_writer << "G1 X" << to_print(p.data[i][j].x) << " Y" << to_print(p.data[i][j].y)<< " E"<<E;
                if(F_for_G1){
                    file_writer << " F600";
                }
                file_writer << std::endl;
                F_for_G1 = false;
            }
        }
    }
}

bool poly_empty(const polylines &p)
{
     for (unsigned int i = 0; i < p.data.size(); i++)
    {
        for (unsigned int j = 0; j < p.data[i].size(); j++)
        {
           return false;
        }
    }
    return true;
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

    
    for (unsigned int i = 0, level = 0; i< obj->get_slice_count(); i++)
    {
        if(poly_empty(obj->get_poly_level(i))){
            continue;
        }
        file_writer << "; Layer[" << level << "]:\n";
        file_writer << "G21 ; mm\n";
        file_writer << "G90 ; abs\n";
        file_writer << "G92 E0 ; reset extr\n";
        E = 0.0f;
        file_writer << "M106 S0\n";
        file_writer << "G0 F300 Z" << 0.300 * (level + 1) << std::endl;
        print_polylines(obj->get_poly_level(i));
        level++;
    }

    file_writer << "G0X0Y0\n";
    file_writer << "M104 S0 ;turn off head heater\n";
    file_writer << "M140 S0 ;turn off bed heater\n";
    file_writer << "M106 S0" << std::endl;
}  
