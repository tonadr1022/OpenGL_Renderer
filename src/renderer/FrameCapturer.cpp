//
// Created by Tony Adriansen on 3/5/24.
//

#include "FrameCapturer.hpp"
#include "src/core/Logger.hpp"

void FrameCapturer::UpdateViewport(uint32_t width, uint32_t height) {
  m_screenTexture.Resize(width, height);
  m_rbo.Bind();
  m_rbo.BufferStorage(width, height, GL_DEPTH24_STENCIL8);
  m_rbo.Unbind();
}

FrameCapturer::FrameCapturer(uint32_t width, uint32_t height)
    : m_screenTexture(width, height) {
  m_screenTexture.Bind();
  m_screenTexture.SetFilterMode(GL_LINEAR, GL_LINEAR);
  m_screenTexture.Unbind();
  m_fbo.Bind();
  m_rbo.Bind();
  m_fbo.AttachColorBuffer(m_screenTexture.Id());
  m_rbo.BufferStorage(width, height, GL_DEPTH24_STENCIL8);
  m_fbo.AttachRenderBuffer(m_rbo.Id());

  if (!m_fbo.IsComplete()) LOG_ERROR("FBO incomplete");
  FrameBuffer::BindDefault();
  m_rbo.Unbind();
}

void FrameCapturer::StartCapture() {
  m_fbo.Bind();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glClearColor(1.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FrameCapturer::EndCapture() {
  FrameBuffer::BindDefault();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}


