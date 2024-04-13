//
// Created by Tony Adriansen on 2/28/2024.
//

#include "VertexArray.hpp"

void VertexArray::Bind() const { glBindVertexArray(m_id); }

void VertexArray::Unbind() { glBindVertexArray(0); }

void VertexArray::AttachBuffer(const size_t id, const BufferType type, const size_t size,
                               const DrawMode mode, const void* data) const noexcept {
  glBindBuffer(type, id);
  glBufferData(type, size, data, mode);
}

VertexArray::VertexArray() { glGenVertexArrays(1, &m_id); }
VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_id); }
