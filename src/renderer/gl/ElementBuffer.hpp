//
// Created by Tony Adriansen on 2/29/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_ELEMENTBUFFER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_ELEMENTBUFFER_HPP_

#include <glad/glad.h>

class ElementBuffer {
 public:
  ElementBuffer();
  ~ElementBuffer();
  void Bind() const noexcept;
  static void Unbind()  noexcept;
  void Delete() noexcept;
//  [[nodiscard]] GLuint Id() const noexcept { return m_id; }

 private:
  GLuint m_id{0};
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_ELEMENTBUFFER_HPP_
