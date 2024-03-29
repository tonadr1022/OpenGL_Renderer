//
// Created by Tony Adriansen on 3/1/24.
//

#include "FPSCamera.hpp"
#include "src/core/Input.hpp"
#include "src/renderer/Renderer.hpp"
#include "src/core/Logger.hpp"
#include <imgui/imgui.h>


void FPSCamera::UpdateMatrices() {
  m_viewMatrix = glm::lookAt(m_pos, m_pos + m_front, UP);
  m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
  m_VPMatrix = m_projectionMatrix * m_viewMatrix;
}
void FPSCamera::Update(double dt) {
  float offset = m_movementSpeed * (float) dt;
  if (Input::IsKeyDown(GLFW_KEY_W) || Input::IsKeyDown(GLFW_KEY_O)) {
    m_pos += m_front * offset;
    m_dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_S) || Input::IsKeyDown(GLFW_KEY_L)) {
    m_pos -= m_front * offset;
    m_dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_D) || Input::IsKeyDown(GLFW_KEY_SEMICOLON)) {
    m_pos += m_right * offset;
    m_dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_A) || Input::IsKeyDown(GLFW_KEY_K)) {
    m_pos -= m_right * offset;
    m_dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_U) || Input::IsKeyDown(GLFW_KEY_R)) {
    m_pos += UP * offset;
    m_dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_J) || Input::IsKeyDown(GLFW_KEY_F)) {
    m_pos -= UP * offset;
    m_dirty = true;
  }
  if (m_dirty) {
    UpdateMatrices();
  }
}

FPSCamera::FPSCamera(float aspectRatio) :
    Camera(aspectRatio),
    m_fov(45.0f),
    m_yaw(-90.0f),
    m_pitch(0),
    m_movementSpeed(DEFAULT_MOVEMENT_SPEED),
    m_mouseSensitivity(DEFAULT_MOUSE_SENSITIVITY) {
  m_front = {0, 0, -1};
  UpdateMatrices();
}

void FPSCamera::ProcessMouseMovement(double xOffset, double yOffset) {
  m_yaw += (float) xOffset * m_mouseSensitivity;
  m_pitch -= (float) yOffset * m_mouseSensitivity; // need to flip yOffset (screen space)
  m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
  glm::vec3 eulers;
  eulers.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  eulers.y = glm::sin(glm::radians(m_pitch));
  eulers.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  m_front = glm::normalize(eulers);
//  LOG_INFO("%f,%f,%f", m_front.x, m_front.y, m_front.z);
  m_right = glm::normalize(glm::cross(m_front, UP));
  UpdateMatrices();
}

void FPSCamera::OnImGui() {
  ImGui::Text("Position: %.1f, %.1f, %.1f", m_pos.x, m_pos.y, m_pos.z);
  ImGui::Text("Front: %.1f, %.1f, %.1f", m_front.x, m_front.y, m_front.z);
  ImGui::Text("Aspect Ratio: %.3f", m_aspectRatio);
  float fovRad = glm::radians(m_fov);
  if (ImGui::SliderAngle("FOV", &fovRad, MIN_FOV, MAX_FOV)) {
    m_fov = glm::degrees(fovRad);
    UpdateMatrices();
  }
  ImGui::SliderFloat("Movement Speed", &m_movementSpeed, MIN_MOVE_SPEED, MAX_MOVE_SPEED);
  ImGui::SliderFloat("Mouse Sensitivity", &m_mouseSensitivity, MIN_MOUSE_SENSITIVITY, MAX_MOUSE_SENSITIVITY);
  if (ImGui::Button("Reset Settings")) {
    m_movementSpeed = DEFAULT_MOVEMENT_SPEED;
    m_mouseSensitivity = DEFAULT_MOUSE_SENSITIVITY;
  }
}

void FPSCamera::OnMouseScrollEvent(double yOffset) {
  m_fov += (float) yOffset;
  m_fov = glm::clamp(m_fov, MIN_FOV, MAX_FOV);
  UpdateMatrices();
}


void FPSCamera::SetTargetPos(const glm::vec3& targetPos) {
  m_viewMatrix = glm::lookAt(m_pos, targetPos, UP);
  m_VPMatrix = m_projectionMatrix * m_viewMatrix;
}