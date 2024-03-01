//
// Created by Tony Adriansen on 2/29/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXBUFFER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXBUFFER_HPP_

#include <glad/glad.h>
#include <cstdint>

namespace GL {
class VertexBuffer {
 public:
  explicit VertexBuffer();
  void Bind() const noexcept;
  void Unbind() const noexcept;
//  [[nodiscard]] GLuint Id() const noexcept { return m_id; }

  template<typename T>
  void EnableAttribute(size_t index, size_t size, size_t stride, const void* ptr) noexcept;

 private:
  GLuint m_id{0};
};
}
#endif //OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXBUFFER_HPP_
