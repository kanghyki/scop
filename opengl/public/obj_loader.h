#ifndef INCLUDED_OBJ_LOADER_H
#define INCLUDED_OBJ_LOADER_H

#include "vertex.h"
#include <string>
#include <vector>

struct Object
{
    std::string name_;

    /* Output */
    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;
    ftm::vec3 center_;

    std::vector<ftm::vec3> vpos_;
    std::vector<ftm::vec3> vnorm_;
    std::vector<ftm::vec2> vuv_;
    std::vector<uint32_t> ipos_;
    std::vector<uint32_t> inorm_;
    std::vector<uint32_t> iuv_;

    std::vector<Object> children_;
};

bool LoadObject(const std::string &in_filepath, Object &out_obj);

#endif
