//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_TRANSFORM_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_TRANSFORM_HPP_

#include <glm/glm.hpp>

class Transform {
 public:
  void Translate(const glm::vec3& offset);
  void Scale(const glm::vec3& vec);
  void Rotate(float degrees, const glm::vec3& axis);
  void SetLocalPos(const glm::vec3& newPos);
  void SetScale(const glm::vec3& scale);
  const glm::vec3& GetPosition();
  const glm::vec3& GetScale();
  const glm::mat4& GetModelMatrix();
  [[nodiscard]] inline bool IsDirty() const { return m_isDirty; }
  void UpdateModelMatrix(const glm::mat4& parentModelMatrix, bool parentDirty);
  void UpdateModelMatrix(bool force = false);

 private:
  glm::vec3 m_pos{0.0f};
  glm::vec3 m_eulerRotDegrees{0.0f};
  glm::vec3 m_scale{1.0f};
  glm::mat4 m_modelMatrix{1.0f};

  bool m_isDirty{false};
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_TRANSFORM_HPP_
