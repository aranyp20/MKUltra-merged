#include <outer_shell_generator.h>

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

outer_shell_generator::outer_shell_generator()
{
}

outer_shell_generator::square::square(const vec2 &_start, double _size) : start(_start), size(_size)
{
}

polylines outer_shell_generator::generate(const std::pair<vec2, double> &bounding_box, double h, unsigned int resolution) const
{
    std::vector<square> leaves = rejection_testing(square(bounding_box.first, bounding_box.second), h, resolution);
    return organiser.organise_sections(generate_contour(leaves, h), h);
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
    return fn_inter(interval(s.start.x, s.start.x + s.size), interval(s.start.y, s.start.y + s.size), h).contains(0);
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
    if (inside(vec3(se.p1.x, se.p1.y, h)) && !inside(vec3(se.p2.x, se.p2.y, h)))
    {
        ip = vec2(se.p1);
        op = vec2(se.p2);
    }
    else if (!inside(vec3(se.p1.x, se.p1.y, h)) && inside(vec3(se.p2.x, se.p2.y, h)))
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

    for (int i = 0; i < 10; i++)
    {
        if (!inside(vec3(mid.x, mid.y, h)))
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
        result.push_back(inside(vec3(tv.x, tv.y, h)));
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

id_section::id_section(const section &s, unsigned int _id_a, unsigned int _id_b) : me(s), id_a(_id_a), id_b(_id_b)
{
}

void section_organiser::push_back_idsections(polylines &result, std::vector<id_section> &current_string, double h) const
{
    std::vector<section> tmp;
    for (const auto &a : current_string)
    {
        tmp.push_back(a.me);
    }
    result.data.push_back(serialize(tmp, h));
}

void section_organiser::create_one_string(polylines &result, std::vector<id_section> &unorganised, double h) const
{
    id_section string_start = unorganised.back();
    unorganised.pop_back();

    std::vector<id_section> current_string;
    current_string.push_back(string_start);

    find_attachments(unorganised, current_string);

    push_back_idsections(result, current_string, h); // ez lehetne polylinesban
}

polylines section_organiser::organise_sections(const std::vector<id_section> &_unorganised, double h) const
{
    polylines result;

    std::vector<id_section> unorganised(_unorganised);

    while (!unorganised.empty())
    {
        create_one_string(result, unorganised, h);
    }

    return result;
}

void section_organiser::find_attachments(std::vector<id_section> &from, std::vector<id_section> &current_string) const
{

    while (try_attach(from, current_string, true))
        ;
    if (try_join_ends(current_string))
        return;
    while (try_attach(from, current_string, false))
        ;
}

bool section_organiser::attach_to_end(std::vector<id_section> &from, std::vector<id_section> &current_string, unsigned int id) const
{
    if (current_string.back().id_b == from[id].id_a)
    {
        attach(from, id, current_string, true);
        return true;
    }
    else if (current_string.back().id_b == from[id].id_b)
    {
        from[id].swap();
        attach(from, id, current_string, true);
        return true;
    }
    return false;
}

bool section_organiser::attach_to_front(std::vector<id_section> &from, std::vector<id_section> &current_string, unsigned int id) const
{
    if (current_string[0].id_a == from[id].id_b)
    {
        attach(from, id, current_string, false);
        return true;
    }
    else if (current_string.back().id_b == from[id].id_b)
    {
        from[id].swap();
        attach(from, id, current_string, false);
        return true;
    }
    return false;
}

bool section_organiser::try_attach(std::vector<id_section> &from, std::vector<id_section> &current_string, bool to_end) const
{
    for (int i = 0; i < from.size(); i++)
    {
        if (to_end)
        {
            if (attach_to_end(from, current_string, i))
                return true;
        }
        else
        {
            if (attach_to_front(from, current_string, i))
                return true;
        }
    }
    return false;
}

void section_organiser::attach(std::vector<id_section> &from, unsigned int what, std::vector<id_section> &current_string, bool to_end) const
{
    if (to_end)
    {
        current_string.push_back(from[what]);
        from.erase(from.begin() + what);
    }
    else
    {

        current_string.insert(current_string.begin(), from[what]);
        from.erase(from.begin() + what);
    }
}

bool section_organiser::try_join_ends(std::vector<id_section> &v) const
{
    return v.back().id_b == v[0].id_a;
}

void id_section::swap()
{
    std::swap(id_a, id_b);
    me.swap_sides();
}
