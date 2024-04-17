//
// Created by Tony Adriansen on 3/1/24.
//

#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    : m_verticesLength(vertices.size()), m_indicesLength(indices.size()) {
  m_vao.Bind();
  m_vbo.Bind();
  m_ebo.Bind();
  m_vao.AttachBuffer(m_vbo.Id(), BufferType::ARRAY, vertices.size() * sizeof(Vertex), STATIC,
                     vertices.data());
  m_vao.AttachBuffer(m_ebo.Id(), BufferType::ELEMENT, indices.size() * sizeof(uint32_t), STATIC,
                     indices.data());
  m_vao.EnableAttribute<float>(0, 3, sizeof(Vertex), (void*)offsetof(Vertex, position));
  m_vao.EnableAttribute<float>(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  m_vao.EnableAttribute<float>(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
}

Mesh::~Mesh() = default;
