//
// Created by Tony Adriansen on 2/28/2024.
//

#include "VertexArray.hpp"
#include "src/core/Logger.hpp"

namespace GL {
void VertexArray::Bind() const noexcept {
  DEBUG_ASSERT(m_id == 0, "Vertex Array Id is valid");

  glBindVertexArray(m_id);
}

void VertexArray::Unbind() noexcept {
  glBindVertexArray(0);
}

void VertexArray::AttachBuffer(const size_t id, const GL::BufferType type,
                               const size_t size,
                               const GL::DrawMode mode,
                               const void* data) const noexcept {
  Bind();
  glBindBuffer(type, id);
  glBufferData(type, size, data, mode);
  Unbind();
}

VertexArray::VertexArray() noexcept {
  glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() noexcept {
  glDeleteVertexArrays(1, &m_id);
}
}
