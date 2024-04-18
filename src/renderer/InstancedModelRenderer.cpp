#include "src/renderer/InstancedModelRenderer.hpp"

#include <iostream>

InstancedModelRenderer::InstancedModelRenderer(const Group* model,
                                               const std::vector<glm::mat4>& model_matrices)
    : m_model(model), m_num_instances(model_matrices.size()) {
  std::cout << model->GetObjects()[0]->GetMesh() << "\n";
  glGenBuffers(1, &m_matrix_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, model_matrices.size() * sizeof(glm::mat4), model_matrices.data(),
               GL_STATIC_DRAW);

  constexpr int SizeOfMatrix = sizeof(glm::mat4);
  constexpr int MatrixStartLoc = 3;
  for (const auto& object : m_model->GetObjects()) {
    const auto& vao = object->GetMesh()->GetVAO();
    vao.Bind();
    for (int i = 0; i < 4; i++) {
      vao.EnableAttribute<float>(i + MatrixStartLoc, 4, SizeOfMatrix,
                                 (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i + MatrixStartLoc, 1);
    }
  }
}

InstancedModelRenderer::~InstancedModelRenderer() { glDeleteBuffers(1, &m_matrix_buffer_id); }
