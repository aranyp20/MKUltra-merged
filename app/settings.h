#pragma ponce

enum surface_type
{
    CHMUTOV = 0,
    GYROID = 1

};

class settings
{

public:
    static unsigned int level_count;
    static unsigned int inner_shell_count;
    static double inner_shell_distance;
    static int s_type;
};
