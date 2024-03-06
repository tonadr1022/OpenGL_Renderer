//
// Created by Tony Adriansen on 3/5/24.
//

#include "RenderBuffer.hpp"
#include "src/core/Logger.hpp"

RenderBuffer::RenderBuffer() = default;

void RenderBuffer::Gen() {
  glGenRenderbuffers(1, &m_id);
}

void RenderBuffer::Bind() const {
  ASSERT(m_id != 0, "Render buffer not valid");
  glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void RenderBuffer::Unbind() {
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::Delete() {
  glDeleteRenderbuffers(1, &m_id);
  m_id = 0;
}

void RenderBuffer::BufferStorage(uint32_t width, uint32_t height, GLint internalFormat) {
  Bind();
  glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
  Unbind();
}
