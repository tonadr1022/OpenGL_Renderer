#include "src/renderer/InstancedModelRenderer.hpp"

InstancedModelRenderer::InstancedModelRenderer(const Group* model,
                                               const std::vector<glm::mat4>& model_matrices)
    : m_model(model), m_num_instances(model_matrices.size()) {
  glGenBuffers(1, &m_matrix_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, model_matrices.size() * sizeof(glm::mat4), model_matrices.data(),
               GL_STATIC_DRAW);

  constexpr int SizeOfMatrix = sizeof(glm::mat4);
  for (const auto& object : m_model->GetObjects()) {
    const auto& vao = object->GetMesh()->GetVAO();
    vao.Bind();

    vao.EnableAttribute<float>(Mesh::FinalVertexAttributeIndex + 1, 4, SizeOfMatrix, nullptr);
    vao.EnableAttribute<float>(Mesh::FinalVertexAttributeIndex + 2, 4, SizeOfMatrix,
                               reinterpret_cast<void*>(sizeof(glm::vec4)));
    vao.EnableAttribute<float>(Mesh::FinalVertexAttributeIndex + 3, 4, SizeOfMatrix,
                               reinterpret_cast<void*>(2 * sizeof(glm::vec4)));
    vao.EnableAttribute<float>(Mesh::FinalVertexAttributeIndex + 4, 4, SizeOfMatrix,
                               reinterpret_cast<void*>(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(Mesh::FinalVertexAttributeIndex + 1, 1);
    glVertexAttribDivisor(Mesh::FinalVertexAttributeIndex + 2, 1);
    glVertexAttribDivisor(Mesh::FinalVertexAttributeIndex + 3, 1);
    glVertexAttribDivisor(Mesh::FinalVertexAttributeIndex + 4, 1);

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
