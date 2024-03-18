//
// Created by Tony Adriansen on 3/17/24.
//

#include "Quad.hpp"

namespace {
const float quadVertices[] = {
    // positions   // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};
}

Quad::Quad() {
  m_vbo.Bind();
  m_vao.Bind();
  m_vao.AttachBuffer(m_vbo.Id(), BufferType::ARRAY, sizeof(quadVertices), STATIC, quadVertices);
  m_vao.EnableAttribute<float>(0, 2, sizeof(float) * 4, nullptr);
  m_vao.EnableAttribute<float>(1, 2, sizeof(float) * 4, (void*) (sizeof(float) * 2));
}
void Quad::Draw() {
  m_vao.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

Quad::~Quad() = default;
