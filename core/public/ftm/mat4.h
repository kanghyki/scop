#ifndef INCLUDED_MAT4_H
#define INCLUDED_MAT4_H

#include "matrix.h"
#include <assert.h>

namespace ftm
{
template <typename T> struct mat<4, 4, T>
{
    typedef mat<4, 4, T> type;
    typedef vec<4, T> col_type;

    col_type value[4];

    size_t constexpr col_length() const
    {
        return 4;
    }
    size_t constexpr row_length() const
    {
        return 4;
    }

    mat() : value{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
    {
    }

    mat(T const &v) : value{{v, v, v, v}, {v, v, v, v}, {v, v, v, v}, {v, v, v, v}}
    {
    }

    mat(type const &m) : value{m[0], m[1], m[2], m[3]}
    {
    }

    mat(col_type const &v0, col_type const &v1, col_type const &v2, col_type const &v3) : value{v0, v1, v2, v3}
    {
    }

    col_type &operator[](size_t i)
    {
        assert(i >= 0 && i < col_length());

        switch (i)
        {
        case 0:
            return value[0];
        case 1:
            return value[1];
        case 2:
            return value[2];
        case 3:
            return value[3];
        default:
            return value[0];
        }
    }

    col_type const &operator[](size_t i) const
    {
        assert(i >= 0 && i < col_length());

        switch (i)
        {
        case 0:
            return value[0];
        case 1:
            return value[1];
        case 2:
            return value[2];
        case 3:
            return value[3];
        default:
            return value[0];
        }
    }
};
}; // namespace ftm

#endif
