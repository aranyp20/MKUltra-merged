#ifndef _SETTINGS_
#define _SETTINGS_

enum surface_type
{
    CHMUTOV = 0,
    GYROID = 1,
    SPHERE = 2,
    CYLINDER = 3

};

class settings
{
public:
    static unsigned int level_count;
    static unsigned int inner_shell_count;
    static double inner_shell_distance;
    static int s_type;
    static double infill_space_between;
    static int infill_number_rot;
};

#endif

/*
BUGS:


TODO:
nem-e problemas alakitgatni vec3 es vec3_t kozott
intervalok kozotti distance nem jo

infillnel bisection method
infillnel a generatorsectionok egy atlobol csinalodjanak
surface ad egy recommendationt es azzal kitolt mindent
polylines eatet hasznalni mindenhol
settingsben ha megvaltozik valami arrol ertesitse a viewt
guiban atnevezesek(snakecase)
2D forgato matrix



statusbar surface betoltesehez

vec2->vec3 illetve az egesz mymath.h-hoz cpp, vec2-re jo normalize-t meg ilyeneket stb stb..
squaret es planet valamennyire ossze lehetne vonni
const iterator
iterator jaitas: it->data
RBF kiertekelst optimalizalni
inner_fill-nel a relevant_section-oket eleg lenne egyszer kiszamolni


DONE:
functioncreater createje ne legyen static


-mostly..:

planet hasznalni mindenhol

*/
