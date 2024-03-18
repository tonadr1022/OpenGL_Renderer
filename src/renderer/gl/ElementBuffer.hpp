//
// Created by Tony Adriansen on 2/29/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_ELEMENTBUFFER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_ELEMENTBUFFER_HPP_

#include "src/Common.hpp"


class ElementBuffer {
 public:
  ElementBuffer();
  ~ElementBuffer();

  void Bind() const;
  void Unbind() const;

  [[nodiscard]] inline GLuint Id() const { return m_id; }

 private:
  GLuint m_id{0};
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_ELEMENTBUFFER_HPP_
