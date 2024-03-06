//
// Created by Tony Adriansen on 3/5/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_RENDERBUFFER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_RENDERBUFFER_HPP_
#include "src/Common.hpp"

class RenderBuffer {
 public:
  RenderBuffer();
  ~RenderBuffer() = default;
  void Gen();
  void Bind() const;
  static void Unbind() ;
  void Delete();
  void BufferStorage(uint32_t width, uint32_t height, GLint internalFormat);
  [[nodiscard]] inline GLuint Id() const {return m_id; }
 private:
  GLuint m_id;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_RENDERBUFFER_HPP_
