#include <section_organiser.h>

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

id_section::id_section(const section &s, unsigned int _id_a, unsigned int _id_b) : me(s), id_a(_id_a), id_b(_id_b)
{
}
