#include "outer_shell_generator.h"

std::map<std::vector<bool>, std::vector<std::array<unsigned int, 2>>> outer_shell_generator::LUT::table = {
    {{false, false, false, false}, {}},
    {{false, false, false, true}, {{2, 3}}},
    {{false, false, true, false}, {{1, 2}}},
    {{false, false, true, true}, {{1, 3}}},
    {{false, true, false, false}, {{0, 1}}},
    {{false, true, false, true}, {{0, 3}, {1, 2}}},
    {{false, true, true, false}, {{0, 2}}},
    {{false, true, true, true}, {{0, 3}}},
    {{true, false, false, false}, {{0, 3}}},
    {{true, false, false, true}, {{0, 2}}},
    {{true, false, true, false}, {{0, 1}, {2, 3}}},
    {{true, false, true, true}, {{0, 1}}},
    {{true, true, false, false}, {{1, 3}}},
    {{true, true, false, true}, {{1, 2}}},
    {{true, true, true, false}, {{2, 3}}},
    {{true, true, true, true}, {}}};

std::vector<std::array<unsigned int, 2>> outer_shell_generator::LUT::find_intersectables(const std::vector<bool> &s)
{
    return table[s];
}

section outer_shell_generator::square::get_section(unsigned int side_index) const
{

    switch (side_index)
    {
    case 0:
        return section(get_vert(0), get_vert(1));
        break;
    case 1:
        return section(get_vert(1), get_vert(2));
        break;
    case 2:
        return section(get_vert(2), get_vert(3));
        break;
    case 3:
        return section(get_vert(3), get_vert(0));
        break;
    default:
        throw std::exception();
        break;
    }
}

vec2 outer_shell_generator::square::get_vert(unsigned int vert_index) const
{
    switch (vert_index)
    {
    case 0:
        return vec2(start.x, start.y);
        break;
    case 1:
        return vec2(start.x + size, start.y);
        break;
    case 2:
        return vec2(start.x + size, start.y + size);
        break;
    case 3:
        return vec2(start.x, start.y + size);
        break;
    default:
        throw std::exception();
        break;
    }
}

outer_shell_generator::outer_shell_generator(std::shared_ptr<frep_object> _surface) : surface(_surface)
{
}

outer_shell_generator::square::square(const vec2 &_start, double _size) : start(_start), size(_size)
{
}

polylines outer_shell_generator::generate(const plane &plane, double h, unsigned int resolution) const
{

    std::vector<square> leaves = rejection_testing(square(plane.get_corner(), plane.get_size()), h, resolution);
    std::vector<id_section> sections = generate_contour(leaves, h);

    auto res = organiser.organise_sections(sections, h);

    return res;
}

void outer_shell_generator::rejection_testing_square(std::vector<square> &leaves, const square &testable, double h, unsigned int resolution) const
{

    if (rejection_test(testable, h))
    {
        if (resolution == 0)
        {

            leaves.push_back(testable);
        }
        else
        {
            std::vector<square> childs_leaves = rejection_testing(testable, h, resolution);
            leaves.insert(leaves.end(), childs_leaves.begin(), childs_leaves.end());
        }
    }
}

std::vector<outer_shell_generator::square> outer_shell_generator::rejection_testing(const outer_shell_generator::square &s, double h, unsigned int resolution) const
{
    resolution -= 1;

    std::vector<square> brokeup = s.breakup(3);
    std::vector<square> leaf_nodes;

    for (int i = 0; i < brokeup.size(); i++)
    {
        rejection_testing_square(leaf_nodes, brokeup[i], h, resolution);
    }

    return leaf_nodes;
}

bool outer_shell_generator::rejection_test(const square &s, double h) const
{
    interval tmp = surface->fn(interval(s.start.x, s.start.x + s.size), interval(s.start.y, s.start.y + s.size), h);
    return tmp.contains(0);
}

std::vector<outer_shell_generator::square> outer_shell_generator::square::breakup(unsigned int count) const
{
    std::vector<outer_shell_generator::square> result;
    double lit_size = size / count;

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            result.push_back(square(vec2(start.x + lit_size * i, start.y + lit_size * j), lit_size));
        }
    }

    return result;
}

std::array<vec2, 2> outer_shell_generator::arrange_inside_outside_point(const section &se, double h) const
{
    vec2 ip, op;
    if (surface->inside(vec3(se.p1.x, se.p1.y, h)) && !surface->inside(vec3(se.p2.x, se.p2.y, h)))
    {
        ip = vec2(se.p1);
        op = vec2(se.p2);
    }
    else if (!surface->inside(vec3(se.p1.x, se.p1.y, h)) && surface->inside(vec3(se.p2.x, se.p2.y, h)))
    {
        ip = vec2(se.p2);
        op = vec2(se.p1);
    }
    else
    {
        throw std::exception();
    }
    return std::array<vec2, 2>{ip, op};
}

vec2 outer_shell_generator::find_zero_value_point(const std::array<vec2, 2> &betweens, double h) const
{
    vec2 ip(betweens[0]);
    vec2 op(betweens[1]);

    vec2 mid = (ip + op) / 2;

    for (int i = 0; i < 3; i++)
    {
        if (!surface->inside(vec3(mid.x, mid.y, h)))
        {
            mid = (ip + mid) / 2;
            op = mid;
        }
        else
        {
            mid = (op + mid) / 2;
            ip = mid;
        }
    }
    return mid;
}

vec2 outer_shell_generator::calc_surfacepoint(const section &se, double h) const
{
    std::array<vec2, 2> end_points = arrange_inside_outside_point(se, h);
    return find_zero_value_point(end_points, h);
}

std::vector<bool> outer_shell_generator::evaluate_verts(const square &s, double h) const
{
    std::vector<bool> result;

    for (int i = 0; i < 4; i++)
    {
        vec2 tv = s.get_vert(i);
        result.push_back(surface->inside(vec3(tv.x, tv.y, h)));
    }

    return result;
}

std::vector<std::array<section, 2>> outer_shell_generator::find_sections_to_ids(const square &sq, const std::vector<std::array<unsigned int, 2>> &ids) const
{
    std::vector<std::array<section, 2>> result;
    for (const auto &b : ids)
    {
        result.push_back({sq.get_section(b[0]), sq.get_section(b[1])});
    }
    return result;
}

std::vector<std::array<unsigned int, 2>> outer_shell_generator::find_section_indexes(const square &sq, section_indexer &indexer, const std::vector<std::array<unsigned int, 2>> ids) const
{
    std::vector<std::array<unsigned int, 2>> result;
    for (const auto &b : ids)
    {
        result.push_back({indexer.PUT_section(sq.get_section(b[0])), indexer.PUT_section(sq.get_section(b[1]))});
    }
    return result;
}

void outer_shell_generator::square_to_idsections_for_contour(const square &sq, std::vector<id_section> &result, section_indexer &indexer, double h) const
{
    std::vector<bool> evaluated_verts = evaluate_verts(sq, h);
    std::vector<std::array<unsigned int, 2>> ids = LUT::find_intersectables(evaluated_verts);
    std::vector<std::array<section, 2>> cutter_sections = find_sections_to_ids(sq, ids);
    std::vector<std::array<unsigned int, 2>> section_indexes = find_section_indexes(sq, indexer, ids);

    std::vector<id_section> ps;
    for (int i = 0; i < cutter_sections.size(); i++)
    {
        id_section temp = id_section(section(calc_surfacepoint(cutter_sections[i][0], h), calc_surfacepoint(cutter_sections[i][1], h)), section_indexes[i][0], section_indexes[i][1]);
        ps.push_back(temp);
    }
    result.insert(result.end(), ps.begin(), ps.end());
}

std::vector<id_section> outer_shell_generator::generate_contour(const std::vector<square> &unrejecteds, double h) const
{
    std::vector<id_section> result;

    section_indexer section_indexer;

    for (const auto &a : unrejecteds)
    {
        square_to_idsections_for_contour(a, result, section_indexer, h);
    }

    return result;
}

unsigned int outer_shell_generator::section_indexer::PUT_section(const section &s)
{
    for (int i = 0; i < found.size(); i++)
    {
        if (same_section(found[i], s))
        {
            return i;
        }
    }

    found.push_back(s);
    return found.size() - 1;
}

bool outer_shell_generator::section_indexer::same_section(const section &s1, const section &s2) const
{
    double close_enough = s1.length() / 10;

    return (((s1.p1 - s2.p1).length() < close_enough && (s1.p2 - s2.p2).length() < close_enough) || ((s1.p2 - s2.p1).length() < close_enough && (s1.p1 - s2.p2).length() < close_enough));
}
