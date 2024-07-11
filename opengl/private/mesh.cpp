#include "mesh.h"

Mesh::Mesh(uint32_t primitive_type)
    : primitive_type_(primitive_type), vertex_array_(nullptr), vertex_buffer_(nullptr), index_buffer_(nullptr)
{
}

Mesh::~Mesh()
{
}

MeshPtr Mesh::Create(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, uint32_t primitive_type)
{
    auto mesh = MeshPtr(new Mesh(primitive_type));
    mesh->Init(vertices, indices);

    return mesh;
}

void Mesh::Init(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
{
    vertex_array_ = VertexArray::Create();
    vertex_buffer_ = Buffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(Vertex), vertices.size());
    index_buffer_ =
        Buffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t), indices.size());
    vertex_array_->SetAttrib(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
    vertex_array_->SetAttrib(1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal));
    vertex_array_->SetAttrib(2, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, uv));
}

void Mesh::Draw(const Program *program) const
{
    vertex_array_->Bind();
    glDrawElements(primitive_type_, index_buffer_->count(), GL_UNSIGNED_INT, 0);
}
