//
// Created by Tony Adriansen on 3/1/24.
//

#include "OrbitCamera.hpp"
#include "imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "src/core/Input.hpp"

OrbitCamera::OrbitCamera(float aspectRatio)
    : Camera(aspectRatio),
      m_target({0, 0, 0}),
      m_up({0, 1, 0}),
      m_distance(10),
      m_azimuthAngle(0),
      m_polarAngle(0),
      m_fov(DEFAULT_FOV) {
  UpdatePosition();
  UpdateMatrices();
}

void OrbitCamera::ProcessMouseMovement(double xOffset, double yOffset) {
  float speed = 0.05;
  if (Input::IsKeyDown(GLFW_KEY_RIGHT_SHIFT)) {
//      glm::vec3 moveOffset = m
// Move target based on camera orientation and mouse offset
    glm::vec3 moveOffset = m_right * static_cast<float>(-xOffset) * m_sensitivity +
        m_up * static_cast<float>(yOffset) * m_sensitivity;

    m_target += moveOffset * speed;
  } else {
    m_azimuthAngle += (float) xOffset * m_sensitivity;
    m_polarAngle += (float) yOffset * m_sensitivity;
    m_polarAngle = glm::clamp(m_polarAngle, -89.0f, 89.0f);
  }

  UpdatePosition();
  UpdateMatrices();
}

void OrbitCamera::OnImGui() {
  ImGui::Text("Position: %.2f, %.2f, %.2f", m_pos.x, m_pos.y, m_pos.z);
  ImGui::Text("Target Position: %.2f, %.2f, %.2f", m_target.x, m_target.y, m_target.z);
  ImGui::Text("Front: %.2f, %.2f, %.2f", m_front.x, m_front.y, m_front.z);
  ImGui::Text("Distance To Target: %.2f", m_distance);
  ImGui::Text("Polar Angle: %.2f", m_polarAngle);
  ImGui::Text("Azimuth Angle: %.2f", m_azimuthAngle);
  float fovRad = glm::radians(m_fov);
  if (ImGui::SliderAngle("FOV", &fovRad, MIN_FOV, MAX_FOV)) {
    m_fov = glm::degrees(fovRad);
    UpdateMatrices();
  }
}

void OrbitCamera::Update(double dt) {
  const float zoomSpeed = 5;
  if (Input::IsKeyDown(GLFW_KEY_H)) {
    m_distance -= zoomSpeed * (float) dt;
    m_dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_Y)) {
    m_distance += zoomSpeed * (float) dt;
    m_dirty = true;
  }

  if (m_dirty) {
    UpdatePosition();
    UpdateMatrices();
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
  m_front = glm::normalize(m_target - m_pos);
  m_right = glm::normalize(glm::cross(m_target - m_pos, GLOBAL_UP));
  m_up = glm::normalize(glm::cross(m_right, m_target - m_pos));
}

void OrbitCamera::OnMouseScrollEvent(double yOffset) {
  m_distance += yOffset * m_scrollSensitivity;
  if (m_distance < MIN_DISTANCE) m_distance = MIN_DISTANCE;
  UpdatePosition();
  UpdateMatrices();
}

void OrbitCamera::ResetSettings() {

}

void OrbitCamera::SetTargetPos(const glm::vec3& targetPos) {
  m_target = targetPos;
  UpdateMatrices();
}

void OrbitCamera::UpdateMatrices() {
  m_viewMatrix = glm::lookAt(m_pos, m_target, GLOBAL_UP);
  m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
  m_VPMatrix = m_projectionMatrix * m_viewMatrix;
}

void OrbitCamera::SetPosition(const glm::vec3& newPos) {
  m_pos = newPos;
  UpdatePosition();
  UpdateMatrices();
}
