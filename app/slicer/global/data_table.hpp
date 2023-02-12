#pragma once
#include <vector>

template <typename T>
struct data_table
{
    std::vector<std::vector<T>> data;
    // kell egy const valtozat
    struct iterator
    {

        std::vector<std::vector<T>> &data;

        bool untouched = true;

        bool has_next()
        {

            if (data.empty() || (it_outer == data.size() - 1 && it_inner == data[data.size() - 1].size()))
                return false;

            if (untouched)
            {
                untouched = false;
                return true;
            }

            if (it_inner == data[it_outer].size())
            {
                it_outer++;
                it_inner = 0;
            }
            else
            {
                it_inner++;
            }

            return true;
        }

        T *get_data() const
        {
            if (data.empty())
                return nullptr;

            return &(data[it_outer][it_inner]);
        }

        iterator(std::vector<std::vector<T>> &_data) : data(_data) {}

    private:
        unsigned int it_outer = 0;
        unsigned int it_inner = 0;
    };

    iterator begin() { return data_table::iterator(data); }

    data_table() {}
    data_table(std::vector<std::vector<T>> &_data) : data(_data) {}
};