#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include "buffer.h"
#include "program.h"
#include "ptrdef.h"
#include "vertex.h"
#include "vertex_array.h"
#include <vector>

CLASS_PTR(Mesh);
class Mesh
{
  public:
    static MeshPtr Create(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,
                          uint32_t primitive_type = GL_TRIANGLES);
    ~Mesh();

    void Draw(const Program *program) const;

  private:
    Mesh(uint32_t primitive_type);
    Mesh(const Mesh &);
    Mesh &operator=(const Mesh &);

    void Init(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

    uint32_t primitive_type_;
    VertexArrayUPtr vertex_array_;
    BufferPtr vertex_buffer_;
    BufferPtr index_buffer_;
};

#endif
