//
// Created by Tony Adriansen on 3/5/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_RENDERBUFFER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_RENDERBUFFER_HPP_
#include "src/Common.hpp"

class RenderBuffer {
 public:
  explicit RenderBuffer(GLint m_internalFormat);
  ~RenderBuffer();

  void Bind() const;
  void Unbind();

  void BufferStorage(uint32_t width, uint32_t height) const;
  void BufferStorageMultiSample(uint32_t width, uint32_t height, uint32_t numSamples) const;
  [[nodiscard]] inline GLuint Id() const { return m_id; }
 private:
  GLuint m_id = 0;
  GLint m_internalFormat;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_RENDERBUFFER_HPP_
