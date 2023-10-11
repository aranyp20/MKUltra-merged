#include "settings.h"

unsigned int settings::level_count = 10;
unsigned int settings::inner_shell_count = 2;
double settings::inner_shell_distance = 0.05;
surface_type settings::s_type = surface_type::CHMUTOV;
double settings::infill_space_between = 0.1;
int settings::infill_number_rot = 3;
double settings::support_space_between = 0.5;
bool settings::as_fdm = false;
double settings::fdm_fullfill_distance = 0.01;
