//
// Created by Tony Adriansen on 2/28/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXARRAY_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXARRAY_HPP_


#include "Types.hpp"
namespace GL {
class VertexArray {
 public:

  VertexArray() noexcept;
  ~VertexArray() noexcept;
  void Bind() const noexcept;
  static void Unbind() noexcept;
  void AttachBuffer(size_t id, BufferType type, size_t size, DrawMode mode, const void* data) const noexcept;

  [[nodiscard]] GLuint Id() const { return m_id; }

 private:
  GLuint m_id{0};
};
}

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXARRAY_HPP_
