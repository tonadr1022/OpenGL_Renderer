#include "src/renderer/InstancedGroup.hpp"

#include "src/resource/MaterialManager.hpp"
#include "src/utils/HashedString.hpp"

InstancedGroup::InstancedGroup(const Group* group, const std::vector<glm::mat4>& model_matrices)
    : m_model(group), m_num_instances(model_matrices.size()) {
  glGenBuffers(1, &m_matrix_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, model_matrices.size() * sizeof(glm::mat4), model_matrices.data(),
               GL_STATIC_DRAW);

  constexpr int SizeOfMatrix = sizeof(glm::mat4);
  constexpr int MatrixStartLoc = 3;
  for (const auto& object : m_model->GetObjects()) {
    // need to copy materials instead of setting just shader name
    auto new_mat_name =
        HashedString((std::string(object->GetMaterial()->name) + "_instanced").c_str());
    MaterialManager::AddMaterial(new_mat_name, *object->GetMaterial());
    object->SetMaterial(MaterialManager::GetMaterial(new_mat_name));
    object->GetMaterial()->type = Material::Type::Instanced;

    // enable matrix vertex attributes
    const auto& vao = object->GetMesh()->GetVAO();
    vao.Bind();
    for (int i = 0; i < 4; i++) {
      vao.EnableAttribute<float>(i + MatrixStartLoc, 4, SizeOfMatrix,
                                 (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(i + MatrixStartLoc, 1);
    }
  }
}

InstancedGroup::~InstancedGroup() { glDeleteBuffers(1, &m_matrix_buffer_id); }
