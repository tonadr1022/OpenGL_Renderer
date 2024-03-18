//
// Created by Tony Adriansen on 2/28/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXARRAY_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXARRAY_HPP_

#include "src/renderer/gl/Types.hpp"
#include <cstddef>
#include <type_traits>


class VertexArray {
 public:

  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;

  void AttachBuffer(size_t id, BufferType type, size_t size, DrawMode mode, const void* data) const noexcept;

  template<typename T>
  void EnableAttribute(size_t index, size_t size, size_t stride, const void* ptr) const {
    glEnableVertexAttribArray(index);
    if constexpr (std::is_floating_point_v<T>) {
      glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, ptr);
    } else if constexpr (std::is_integral_v<T>) {
      if constexpr (std::is_signed_v<T>) {
        glVertexAttribIPointer(index, size, GL_INT, stride, ptr);
      } else {
        glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride, ptr);
      }
    }
  }

  [[nodiscard]] inline GLuint Id() const { return m_id; }

 private:
  GLuint m_id{0};
};


#endif //OPENGL_RENDERER_SRC_RENDERER_GL_VERTEXARRAY_HPP_
