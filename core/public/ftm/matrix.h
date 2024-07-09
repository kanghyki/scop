#ifndef INCLUDED_MATRIX_H
#define INCLUDED_MATRIX_H

#include "vector.h"

namespace ftm
{
template <size_t R, size_t C, typename T> struct mat;

typedef mat<4, 4, float> mat4;

template <size_t R, size_t C, typename T> const T *value_ptr(const mat<R, C, T> &m)
{
    return &m[0][0];
}
}; // namespace ftm

#include <iostream>
#define VEC_UNION_CHAR "xyzw"

template <size_t R, size_t C, typename T> std::ostream &operator<<(std::ostream &stream, ftm::mat<R, C, T> &m)
{
    stream << "mat" << m.col_length() << "x" << m.row_length() << " =\n";
    for (size_t i = 0; i < m.col_length(); ++i)
    {
        stream << "[ ";
        for (size_t j = 0; j < m.row_length(); ++j)
        {
            stream << " " << VEC_UNION_CHAR[j] << i << ":" << m[i][j];
        }
        stream << " ]\n";
    }

    return stream;
}

#endif
