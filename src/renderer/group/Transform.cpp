//
// Created by Tony Adriansen on 3/2/24.
//

#include "Transform.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "src/core/Logger.hpp"

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

void Transform::UpdateModelMatrix(bool force) {
  if (!m_isDirty && !force) return;
  m_modelMatrix = glm::translate(glm::mat4{1.0f}, m_pos) * glm::mat4_cast(glm::quat(glm::radians(m_eulerRotDegrees)))
      * glm::scale(glm::mat4{1.0f}, m_scale);
  m_isDirty = false;
}

const glm::mat4& Transform::GetModelMatrix() {
  ASSERT(!m_isDirty, "dirty, debug error");
  return m_modelMatrix;
}

const glm::vec3& Transform::GetPosition() {
  return m_pos;
}

const glm::vec3& Transform::GetScale() {
  return m_scale;
}

void Transform::UpdateModelMatrix(const glm::mat4& parentModelMatrix, bool parentDirty) {
  if (m_isDirty || parentDirty) {
    UpdateModelMatrix();
    if (parentDirty) {
      UpdateModelMatrix(true);
      m_modelMatrix = parentModelMatrix * m_modelMatrix;
      m_isDirty = false;
    }
  }
//  m_modelMatrix = parentModelMatrix * (glm::translate(glm::mat4{1.0f}, m_pos) * glm::mat4_cast(glm::quat(glm::radians(m_eulerRotDegrees)))
//      * glm::scale(glm::mat4{1.0f}, m_scale));
//  m_isDirty = false;
}
