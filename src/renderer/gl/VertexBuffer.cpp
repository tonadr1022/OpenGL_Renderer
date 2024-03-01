//
// Created by Tony Adriansen on 2/29/2024.
//

#include "VertexBuffer.hpp"
#include <type_traits>

namespace GL {
VertexBuffer::VertexBuffer() {
  glGenBuffers(1, &m_id);

//  Bind();
//  if (defaultStruct) {
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DefaultVertex), (void*) offsetof(DefaultVertex, x));
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DefaultVertex), (void*) offsetof(DefaultVertex, u));
//    glEnableVertexAttribArray(1);
//    Unbind();
//  }
}

void VertexBuffer::Bind() const noexcept{
  glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::Unbind() const noexcept {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void VertexBuffer::EnableAttribute(size_t index, size_t size, size_t stride, const void* ptr) noexcept {
  if constexpr (std::is_floating_point_v<T>) {
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, ptr);
  } else if constexpr (std::is_integral_v<T>) {
    if constexpr (std::is_signed_v<T>) {
      glVertexAttribIPointer(index, size, GL_INT, stride, ptr);
    } else {
      glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride, ptr);
    }
  }
  glEnableVertexAttribArray(index);
}

}

