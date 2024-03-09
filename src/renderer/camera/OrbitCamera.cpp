//
// Created by Tony Adriansen on 3/1/24.
//

#include "OrbitCamera.hpp"
#include "imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "src/renderer/Input.hpp"

OrbitCamera::OrbitCamera(float aspectRatio)
    : Camera(aspectRatio),
      m_target({0, 0, 0}),
      m_up({0, 1, 0}),
      m_distance(1),
      m_azimuthAngle(0),
      m_polarAngle(0),
      m_fov(DEFAULT_FOV) {
  UpdatePosition();
  UpdateViewMatrix();
  UpdateProjectionMatrix();
}

void OrbitCamera::ProcessMouseMovement(double xOffset, double yOffset) {
  m_azimuthAngle += (float) xOffset * m_sensitivity;
  m_polarAngle += (float) yOffset * m_sensitivity;
  m_polarAngle = glm::clamp(m_polarAngle, -89.0f, 89.0f);

  UpdatePosition();
  UpdateViewMatrix();
  UpdateProjectionMatrix();
}

void OrbitCamera::OnImGui() {
  ImGui::Text("Position: %.2f, %.2f, %.2f", m_pos.x, m_pos.y, m_pos.z);
  ImGui::Text("Target: %.2f, %.2f, %.2f", m_target.x, m_target.y, m_target.z);
  ImGui::Text("Distance: %.2f", m_distance);
  ImGui::Text("Polar Angle: %.2f", m_polarAngle);
  ImGui::Text("Azimuth Angle: %.2f", m_azimuthAngle);
  float fovRad = glm::radians(m_fov);
  if (ImGui::SliderAngle("FOV", &fovRad, MIN_FOV, MAX_FOV)) {
    m_fov = glm::degrees(fovRad);
    UpdateProjectionMatrix();
  }
}

void OrbitCamera::SetTarget(const glm::vec3& target) {
  if (m_target != target) {
    m_target = target;
    // TODO dirty flag?
    UpdateViewMatrix();
  }

}

void OrbitCamera::Update(double dt) {
  const float zoomSpeed = 5;
  bool dirty = false;
  if (Input::IsKeyDown(GLFW_KEY_H)) {
    m_distance -= zoomSpeed * (float) dt;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_Y)) {
    m_distance += zoomSpeed * (float) dt;
    dirty = true;
  }

  if (dirty) {
    UpdatePosition();
    UpdateViewMatrix();
  }
}

void OrbitCamera::UpdatePosition() {
  float azimuthRad = glm::radians(m_azimuthAngle);
  float polarRad = glm::radians(m_polarAngle);
  float sinPolar = glm::sin(polarRad);
  float cosPolar = glm::cos(polarRad);
  float cosAzimuth = glm::cos(azimuthRad);
  float sinAzimuth = glm::sin(azimuthRad);
  m_pos.x = m_target.x + m_distance * cosPolar * cosAzimuth;
  m_pos.y = m_target.y + m_distance * sinPolar;
  m_pos.z = m_target.z + m_distance * sinAzimuth * cosPolar;
}

void OrbitCamera::OnMouseScrollEvent(double yOffset) {
//  m_fov -= (float) yOffset;
//  m_fov = glm::clamp(m_fov, 5.f, 120.f);
  m_distance += yOffset * m_scrollSensitivity;
  if (m_distance < MIN_DISTANCE) m_distance = MIN_DISTANCE;
  UpdatePosition();
  UpdateProjectionMatrix();
  UpdateViewMatrix();
}

void OrbitCamera::ResetSettings() {

}
void OrbitCamera::UpdateViewMatrix() {
  m_viewMatrix = glm::lookAt(m_pos, m_target, m_up);
  m_VPMatrix = m_projectionMatrix * m_viewMatrix;
}
void OrbitCamera::UpdateProjectionMatrix() {
  m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
  m_VPMatrix = m_projectionMatrix * m_viewMatrix;
}

void OrbitCamera::SetTargetPos(const glm::vec3& targetPos) {
  m_viewMatrix = glm::lookAt(m_pos, targetPos, m_up);
  m_VPMatrix = m_projectionMatrix * m_viewMatrix;
}
