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
  void Update(double dt) override;
  void ProcessMouseMovement(double xOffset, double yOffset) override;
  void OnMouseScrollEvent(double yOffset) override;
  void ResetSettings();
  void OnImGui() override;
  void SetTargetPos(const glm::vec3& targetPos) override;

 private:
  static constexpr const glm::vec3 UP = {0, 1, 0};
  void UpdateMatrices();
  float m_fov;
  glm::vec3 m_right{};

  float m_yaw, m_pitch;
  float m_movementSpeed, m_mouseSensitivity;

  static constexpr float MinMoveSpeed = 0.1f;
  static constexpr float MaxMoveSpeed = 200.0f;
  static constexpr float MinMouseSensitivity = 0.01f;
  static constexpr float MaxMouseSensitivity = 1.0f;
  static constexpr float DefaultMovementSpeed = 5.0f;
  static constexpr float DefaultMouseSensitivity = 0.1f;
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_CAMERA_FPSCAMERA_HPP_
