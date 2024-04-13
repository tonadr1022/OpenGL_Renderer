//
// Created by Tony Adriansen on 3/5/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERA_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERA_HPP_

#include <glm/glm.hpp>

class Camera {
 public:
  explicit Camera(float aspectRatio);
  virtual ~Camera() = default;
  virtual void Update(double dt);
  virtual void SetPosition(const glm::vec3& newPos);
  virtual void SetTargetPos(const glm::vec3& targetPos) = 0;
  [[nodiscard]] inline const glm::vec3& GetPosition() const { return m_pos; };
  [[nodiscard]] inline const glm::mat4& GetProjectionMatrix() const {
    return m_projectionMatrix;
  }
  [[nodiscard]] inline const glm::mat4& GetViewMatrix() const {
    return m_viewMatrix;
  }
  [[nodiscard]] inline const glm::mat4& GetVPMatrix() const {
    return m_VPMatrix;
  }
  [[nodiscard]] inline const glm::vec3& GetFront() const { return m_front; }

  void SetAspectRatio(float aspectRatio);
  virtual void OnImGui() = 0;
  virtual void ProcessMouseMovement(double xOffset, double yOffset) = 0;
  virtual void OnMouseScrollEvent(double yOffset) = 0;

 protected:
  glm::vec3 m_pos;
  glm::mat4 m_viewMatrix = {1.0f};
  glm::mat4 m_projectionMatrix = {1.0f};
  glm::mat4 m_VPMatrix = {1.0f};

  glm::vec3 m_front;
  bool m_dirty = false;

  float m_aspectRatio;
  float m_nearPlane, m_farPlane;
  //  static constexpr float SCROLL_SENSITIVITY = 1.0f;
  static constexpr float DefaultFov = 45.0f;
  static constexpr float MaxFov = 120.0f;
  static constexpr float MinFov = 5.0f;
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERA_HPP_
