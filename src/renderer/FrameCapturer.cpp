//
// Created by Tony Adriansen on 3/5/24.
//

#include "FrameCapturer.hpp"
#include "src/core/Logger.hpp"

void FrameCapturer::UpdateViewport(uint32_t width, uint32_t height) {
  m_screenTexture.Resize(width, height);
  m_rbo.BufferStorage(width, height, GL_DEPTH24_STENCIL8);
}

FrameCapturer::FrameCapturer(uint32_t width, uint32_t height)
    : m_screenTexture(width, height, false) {
  m_screenTexture.Bind();
  m_screenTexture.SetFilterMode(GL_LINEAR, GL_LINEAR);
  m_fbo.Gen();
  m_fbo.Bind();
  m_fbo.AttachColorBuffer(m_screenTexture.Id());
  m_rbo.Gen();
  m_rbo.BufferStorage(width, height, GL_DEPTH24_STENCIL8);
  m_fbo.AttachRenderBuffer(m_rbo.Id());
  if (!m_fbo.IsComplete()) LOG_ERROR("FBO incomplete");
  m_fbo.Unbind();
}

void FrameCapturer::StartCapture() {
  m_fbo.Bind();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameCapturer::EndCapture() {
  m_fbo.Unbind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


