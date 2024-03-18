//
// Created by Tony Adriansen on 2/28/2024.
//

#include "VertexArray.hpp"
#include "src/core/Logger.hpp"



void VertexArray::Bind() const {
  ASSERT(m_id != 0, "Vertex Array Id is valid");
  glBindVertexArray(m_id);
}

void VertexArray::Unbind() const {
  glBindVertexArray(0);
}

void VertexArray::AttachBuffer(const size_t id, const BufferType type,
                               const size_t size,
                               const DrawMode mode,
                               const void* data) const {
  glBindBuffer(type, id);
  glBufferData(type, size, data, mode);
}

VertexArray::VertexArray() {
  glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_id);
}




