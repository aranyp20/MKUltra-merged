#pragma once

#include <data_table.hpp>
#include <polylines.h>

template <typename T>
struct opt_return
{

    T data;
    bool valid;

    opt_return(const T &_data, bool _valid = true) : data(_data), valid(_valid) {}
    opt_return() : valid(false) {}
};