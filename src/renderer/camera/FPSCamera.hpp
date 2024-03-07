//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_CAMERA_FPSCAMERA_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_CAMERA_FPSCAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

class FPSCamera : public Camera {
 public:

  explicit FPSCamera(float aspectRatio);
  ~FPSCamera() override = default;
  void SetPosition(const glm::vec3& newPos) override;
  void Update(double dt) override;
  void ProcessMouseMovement(float xOffset, float yOffset);
  void ResetSettings();
  void OnImGui();

  [[nodiscard]] inline const glm::vec3& GetPosition() const override { return m_pos; };
  [[nodiscard]] inline const glm::mat4& GetProjectionMatrix() const override { return m_projectionMatrix; }
  [[nodiscard]] inline const glm::mat4& GetViewMatrix() const override { return m_viewMatrix; }
  [[nodiscard]] inline const glm::mat4& GetVPMatrix() const override { return m_VPMatrix; }
  void SetAspectRatio(float aspectRatio) override;

 private:
  static constexpr const glm::vec3 UP = {0, 1, 0};
  void UpdateMatrices();

  float m_nearPlane, m_farPlane, m_fov, m_aspectRatio;
  glm::vec3 m_pos;
  glm::vec3 m_right;
  glm::vec3 m_up;
  glm::vec3 m_front;
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projectionMatrix{1.0f};
  glm::mat4 m_VPMatrix{};

  float m_yaw, m_pitch;
  float m_movementSpeed, m_mouseSensitivity;

  static constexpr float MIN_MOVE_SPEED = 0.1f;
  static constexpr float MAX_MOVE_SPEED = 200.0f;
  static constexpr float MIN_MOUSE_SENSITIVITY = 0.01f;
  static constexpr float MAX_MOUSE_SENSITIVITY = 1.0f;
  static constexpr float DEFAULT_MOVEMENT_SPEED = 5.0f;
  static constexpr float DEFAULT_MOUSE_SENSITIVITY = 0.1f;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_CAMERA_FPSCAMERA_HPP_
