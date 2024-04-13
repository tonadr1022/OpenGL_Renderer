//
// Created by Tony Adriansen on 2/29/2024.
//

#include "ElementBuffer.hpp"

void ElementBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

ElementBuffer::ElementBuffer() { glGenBuffers(1, &m_id); }
ElementBuffer::~ElementBuffer() { glDeleteBuffers(1, &m_id); }
