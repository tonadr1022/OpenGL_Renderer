//
// Created by Tony Adriansen on 3/2/24.
//

#include "Group.hpp"
#include "src/utils/Logger.hpp"
void Group::AddObject(std::unique_ptr<Object> object) {
  m_objects.push_back(std::move(object));
}

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

void Group::UpdateTransforms() {
//  if (transform.IsDirty()) {
//    transform.UpdateModelMatrix();
//    auto& modelMatrix = transform.GetModelMatrix();
//    for (auto& obj : m_objects) {
//      obj->transform.UpdateModelMatrix(modelMatrix, true);
//    }
//  } else {
//    auto& modelMatrix = transform.GetModelMatrix();
//    for (auto& obj : m_objects) {
//      if (obj->transform.IsDirty()) {
//        obj->transform.UpdateModelMatrix(modelMatrix, false);
//      }
//    }
//  }

  bool parentDirty = transform.IsDirty();
  transform.UpdateModelMatrix();
  auto& modelMatrix = transform.GetModelMatrix();

  for (auto& obj : m_objects) {
    obj->transform.UpdateModelMatrix(modelMatrix, parentDirty || obj->transform.IsDirty());
  }
}