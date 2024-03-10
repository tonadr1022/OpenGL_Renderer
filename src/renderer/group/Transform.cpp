//
// Created by Tony Adriansen on 3/2/24.
//

#include "Transform.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "src/core/Logger.hpp"

//void Transform::ComputeModelMatrix() {
//  UpdateModelMatrix();
//}

//void Transform::ComputeModelMatrix(const glm::mat4& parentModelMatrix) {
//  UpdateModelMatrix();
//  m_modelMatrix = parentModelMatrix * m_modelMatrix;
//}

void Transform::Translate(const glm::vec3& vec) {
  m_pos += vec;
  m_isDirty = true;
}

void Transform::Scale(const glm::vec3& vec) {
  m_scale *= vec;
  m_isDirty = true;
}

void Transform::SetScale(const glm::vec3& newScale) {
  m_scale = newScale;
  m_isDirty = true;
}

void Transform::Rotate(float degrees, const glm::vec3& axis) {
  // I still need to learn quaternions
  glm::quat rotationQuat = glm::quat(glm::radians(m_eulerRotDegrees));
  rotationQuat = glm::rotate(rotationQuat, glm::radians(degrees), axis);
  m_eulerRotDegrees = glm::degrees(glm::eulerAngles(rotationQuat));
  m_isDirty = true;
}

void Transform::SetLocalPos(const glm::vec3& newPos) {
  m_pos = newPos;
  m_isDirty = true;
}

void Transform::UpdateModelMatrix() {
  // Translate Rotate Scale
  if (!m_isDirty) return;
  m_modelMatrix = glm::translate(glm::mat4{1.0f}, m_pos) * glm::mat4_cast(glm::quat(glm::radians(m_eulerRotDegrees)))
      * glm::scale(glm::mat4{1.0f}, m_scale);

//  if (!m_children.empty()) {
//    for (Transform* child : m_children) {
//      ASSERT(child != nullptr, "Null transform, didn't notify this transform on deletion of child")
//      child->ComputeModelMatrix(m_modelMatrix);
//    }
//  }
  m_isDirty = false;
}

const glm::mat4& Transform::GetModelMatrix() {
  ASSERT(!m_isDirty, "dirty, debug error");
  return m_modelMatrix;
}
