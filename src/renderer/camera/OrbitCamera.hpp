//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_CAMERA_ORBITCAMERA_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_CAMERA_ORBITCAMERA_HPP_

#include "Camera.hpp"

class OrbitCamera : public Camera {
 public:
  explicit OrbitCamera(float aspectRatio);
  ~OrbitCamera() override = default;
  void SetTarget(const glm::vec3& target);
  void OnImGui() override;
  void Update(double dt) override;
  void ResetSettings();
  void SetPosition(const glm::vec3& newPos) override;
  void ProcessMouseMovement(double xOffset, double yOffset) override;
  void OnMouseScrollEvent(double yOffset) override;
  void SetTargetPos(const glm::vec3& targetPos) override;

 private:
void UpdatePosition();
void UpdateMatrices();

  glm::vec3 m_target;
  glm::vec3 m_up;


  float m_distance;
  float m_azimuthAngle, m_polarAngle;
  float m_fov;
  float m_scrollSensitivity = DEFAULT_ORBIT_SCROLL_SENSITIVITY;
  float m_sensitivity = 0.5;
  static constexpr float MIN_DISTANCE = 0.5f;
  static constexpr float DEFAULT_ORBIT_SCROLL_SENSITIVITY = 0.25f;

};

#endif //OPENGL_RENDERER_SRC_RENDERER_CAMERA_ORBITCAMERA_HPP_
