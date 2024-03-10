//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_TRANSFORM_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_TRANSFORM_HPP_

#include "glm/glm.hpp"
#include <vector>

class Transform {
 public:
  void Translate(const glm::vec3& offset);
  void Scale(const glm::vec3& scale);
  void Rotate(float radians, const glm::vec3& axis);
  void SetLocalPos(const glm::vec3& newPos);
  void SetScale(const glm::vec3& newScale);

  const glm::mat4& GetModelMatrix();
  [[nodiscard]] inline bool IsDirty() const { return m_isDirty; }

//  void ComputeModelMatrix(const glm::mat4& parentModelMatrix);
//  void ComputeModelMatrix();
//  void AddChild(Transform *child);
//  void RemoveChild(Transform *child);
  void UpdateModelMatrix();
 private:
//  Transform* parent;
//  std::vector<Transform*> m_children;

  glm::vec3 m_pos{0.0f};
  glm::vec3 m_eulerRotDegrees{0.0f};
  glm::vec3 m_scale{1.0f};

  glm::mat4 m_modelMatrix{1.0f};

  bool m_isDirty{false};

};


#endif //OPENGL_RENDERER_SRC_RENDERER_TRANSFORM_HPP_
