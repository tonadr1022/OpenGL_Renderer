//
// Created by Tony Adriansen on 3/5/24.
//

#include "RenderBuffer.hpp"

#include "src/utils/Logger.hpp"

RenderBuffer::RenderBuffer(GLint internalFormat)
    : m_internalFormat(internalFormat) {
  glGenRenderbuffers(1, &m_id);
}

RenderBuffer::~RenderBuffer() { glDeleteRenderbuffers(1, &m_id); }

void RenderBuffer::Bind() const {
  ASSERT(m_id != 0, "Render buffer not valid");
  glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void RenderBuffer::Unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

void RenderBuffer::BufferStorage(uint32_t width, uint32_t height) const {
  glRenderbufferStorage(GL_RENDERBUFFER, m_internalFormat, width, height);
}

void RenderBuffer::BufferStorageMultiSample(uint32_t width, uint32_t height,
                                            uint32_t numSamples) const {
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples,
                                   m_internalFormat, width, height);
}
