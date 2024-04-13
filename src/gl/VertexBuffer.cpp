//
// Created by Tony Adriansen on 2/29/2024.
//

#include "VertexBuffer.hpp"

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

void VertexBuffer::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
VertexBuffer::VertexBuffer() { glGenBuffers(1, &m_id); }

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_id); }
