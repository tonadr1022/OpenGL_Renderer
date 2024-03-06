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
  FPSCamera();
  ~FPSCamera() = default;
  void SetPosition(const glm::vec3& newPos) override;
  void Update(double dt) override;

  [[nodiscard]] inline const glm::vec3& GetPosition() const override { return m_pos; };
  [[nodiscard]] inline const glm::mat4& GetProjectionMatrix() const override { return m_projectionMatrix; }
  [[nodiscard]] inline const glm::mat4& GetViewMatrix() const override { return m_viewMatrix; }
  [[nodiscard]] inline const glm::mat4& GetVPMatrix() const override { return m_VPMatrix; }
  void SetAspectRatio(float aspectRatio) override;

 private:
  static constexpr const glm::vec3 UP = {0, 1, 0};
  void UpdateMatrices();
  float m_movementSpeed = 5;
  float m_nearPlane, m_farPlane, m_fov, m_aspectRatio;
  glm::vec3 m_pos;
  glm::vec3 m_front;
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projectionMatrix{1.0f};
  glm::mat4 m_VPMatrix{};

  float m_yaw, m_pitch;


};

#endif //OPENGL_RENDERER_SRC_RENDERER_CAMERA_FPSCAMERA_HPP_
