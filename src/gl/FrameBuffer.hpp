//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_FRAMEBUFFER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_FRAMEBUFFER_HPP_

#include "src/Common.hpp"

class FrameBuffer {
 public:
  FrameBuffer();
  ~FrameBuffer();
  void Bind() const;
  static void BindDefault();
  [[nodiscard]] bool IsComplete() const;
  void AttachColorBuffer(GLuint textureColorBuffer);
  void AttachColorBuffer(GLenum attachment, GLenum textureTarget,
                         GLuint textureId);
  void AttachRenderBuffer(GLuint rbo);
  [[nodiscard]] inline GLuint Id() const { return m_id; }

 private:
  GLuint m_id{0};
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_GL_FRAMEBUFFER_HPP_
