#include "src/renderer/InstancedModelRenderer.hpp"

#include <iostream>

InstancedModelRenderer::InstancedModelRenderer(const Group* model,
                                               const std::vector<glm::mat4>& model_matrices)
    : m_model(model), m_num_instances(model_matrices.size()) {
  static int c = 0;
  std::cout << model->GetObjects()[0]->GetMesh() << "\n";
  glGenBuffers(1, &m_matrix_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, model_matrices.size() * sizeof(glm::mat4), model_matrices.data(),
               GL_STATIC_DRAW);

  constexpr int SizeOfMatrix = sizeof(glm::mat4);
  constexpr int MatrixStartLoc = 3;
  int i = 0;
  for (const auto& object : m_model->GetObjects()) {
    const auto& vao = object->GetMesh()->GetVAO();
    vao.Bind();
    for (int i = 0; i < 4; i++) {
      vao.EnableAttribute<float>(i + MatrixStartLoc, 4, SizeOfMatrix,
                                 (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i + MatrixStartLoc, 1);
      std::cout << "c: " << c++ << "\n";
    }

    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    // glEnableVertexAttribArray(4);
    // glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
    // (void*)(sizeof(glm::vec4))); glEnableVertexAttribArray(5); glVertexAttribPointer(5, 4,
    // GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
    //                       (void*)(2 * sizeof(glm::vec4)));
    // glEnableVertexAttribArray(6);
    // glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
    //                       (void*)(3 * sizeof(glm::vec4)));
    //
    // glVertexAttribDivisor(3, 1);
    // glVertexAttribDivisor(4, 1);
    // glVertexAttribDivisor(5, 1);
    // glVertexAttribDivisor(6, 1);
  }
}

InstancedModelRenderer::~InstancedModelRenderer() { glDeleteBuffers(1, &m_matrix_buffer_id); }
