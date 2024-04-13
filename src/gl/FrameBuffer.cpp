//
// Created by Tony Adriansen on 3/1/24.
//

#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer() { glGenFramebuffers(1, &m_id); }

void FrameBuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_id); }

bool FrameBuffer::IsComplete() const {
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::AttachColorBuffer(GLuint textureColorBuffer) {
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer,
                         0);
}

void FrameBuffer::AttachColorBuffer(GLenum attachment, GLenum textureTarget, GLuint textureId) {
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureTarget, textureId, 0);
}

void FrameBuffer::AttachRenderBuffer(GLuint rbo) {
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

FrameBuffer::~FrameBuffer() { glDeleteFramebuffers(1, &m_id); }

void FrameBuffer::BindDefault() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
