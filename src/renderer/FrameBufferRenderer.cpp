//
// Created by Tony Adriansen on 3/5/24.
//

#include "FrameBufferRenderer.hpp"
#include "src/core/Logger.hpp"

void FrameBufferRenderer::UpdateViewport(uint32_t width, uint32_t height) {
  m_texture.Bind();
  m_texture.Resize(width, height);
  m_rbo.Bind();
  m_rbo.BufferStorage(width, height, GL_DEPTH24_STENCIL8);
}

FrameBufferRenderer::FrameBufferRenderer(uint32_t width, uint32_t height)
    : m_texture(width, height) {
  m_texture.Bind();
  m_texture.SetFilterMode(GL_LINEAR, GL_LINEAR);
  m_fbo.Bind();
  m_fbo.AttachColorBuffer(m_texture.Id());
  m_rbo.Bind();
  m_rbo.BufferStorage(width, height, GL_DEPTH24_STENCIL8);
  m_fbo.AttachRenderBuffer(m_rbo.Id());
  if (!m_fbo.IsComplete()) LOG_ERROR("FBO incomplete");
}

void FrameBufferRenderer::StartCapture() {
  m_fbo.Bind();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1,0.1,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBufferRenderer::EndCapture() {
  m_fbo.Unbind();
  glClearColor(0.0f,1.0f,1.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}


