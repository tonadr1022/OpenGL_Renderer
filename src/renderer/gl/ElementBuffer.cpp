//
// Created by Tony Adriansen on 2/29/2024.
//

#include "ElementBuffer.hpp"
#include "src/core/Logger.hpp"

void ElementBuffer::Generate() {
  glGenBuffers(1, &m_id);
}

void ElementBuffer::Bind() const {
  ASSERT(m_id != 0, "EBO id valid");
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::Delete()  {
  glDeleteBuffers(1, &m_id);
  m_id = 0;
}
