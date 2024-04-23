//
// Created by Tony Adriansen on 3/2/24.
//

#include "Group.hpp"

void Group::AddObject(std::unique_ptr<Object> object) { m_objects.push_back(std::move(object)); }

Group::Group(bool backFaceCull) : backFaceCull(backFaceCull) {}

void Group::RemoveObject(const Object* object) {
  int i = 0;
  for (auto& o : m_objects) {
    if (o.get() == object) {
      m_objects.erase(m_objects.begin() + i);
      break;
    }
    i++;
  }
}
void Group::SetMaterial(Material* material) {
  for (const auto& obj : m_objects) {
    obj->SetMaterial(material);
  }
}

void Group::UpdateTransforms() {
  bool parent_dirtyy = transform.IsDirty();
  transform.UpdateModelMatrix();
  const auto& model_matrix = transform.GetModelMatrix();

  for (auto& obj : m_objects) {
    obj->transform.UpdateModelMatrix(model_matrix, parent_dirtyy || obj->transform.IsDirty());
  }
}
