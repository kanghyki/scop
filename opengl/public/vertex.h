#ifndef INCLUDED_VERTEX_H
#define INCLUDED_VERTEX_H

#include "ftm.h"

struct Vertex
{
    ftm::vec3 position;
    ftm::vec3 normal;
    ftm::vec2 uv;
};

#endif
