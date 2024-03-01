//
// Created by Tony Adriansen on 2/29/2024.
//

#include "ElementBuffer.hpp"

ElementBuffer::ElementBuffer() {
  glGenBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_id);
}

void ElementBuffer::Bind() const noexcept {

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::Unbind() noexcept {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::Delete() noexcept {
  glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_id);
  m_id = 0;
}

ElementBuffer::~ElementBuffer() {
  Delete();
}

