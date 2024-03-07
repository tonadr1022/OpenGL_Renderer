//
// Created by Tony Adriansen on 3/1/24.
//

#include "FPSCamera.hpp"
#include "src/renderer/Input.hpp"
#include "src/renderer/Renderer.hpp"
#include "src/core/Logger.hpp"
#include <imgui/imgui.h>

void FPSCamera::SetPosition(const glm::vec3& newPos) {
  m_pos = newPos;
}

void FPSCamera::Update(double dt) {
  float offset = m_movementSpeed * (float) dt;
  bool dirty = false;
  if (Input::IsKeyDown(GLFW_KEY_W) || Input::IsKeyDown(GLFW_KEY_O)) {
    m_pos += m_front * offset;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_S) || Input::IsKeyDown(GLFW_KEY_L)) {
    m_pos -= m_front * offset;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_D) || Input::IsKeyDown(GLFW_KEY_SEMICOLON)) {
    m_pos += m_right * offset;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_A) || Input::IsKeyDown(GLFW_KEY_K)) {
    m_pos -= m_right * offset;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_U)) {
    m_pos += UP * offset;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_J)) {
    m_pos -= UP * offset;
    dirty = true;
  }
  if (dirty) UpdateMatrices();
}

FPSCamera::FPSCamera(float aspectRatio) :
    m_nearPlane(0.1),
    m_farPlane(1000),
    m_fov(45.0f),
    m_aspectRatio(aspectRatio),
    m_pos({0, 0, 3}),
    m_front({0, 0, -1}),
    m_yaw(-90.0f),
    m_pitch(0),
    m_movementSpeed(DEFAULT_MOVEMENT_SPEED),
    m_mouseSensitivity(DEFAULT_MOUSE_SENSITIVITY) {
  UpdateMatrices();
}

void FPSCamera::SetAspectRatio(float aspectRatio) {
  if (m_aspectRatio != aspectRatio) {
    m_aspectRatio = aspectRatio;
    UpdateMatrices();
  }
}

void FPSCamera::UpdateMatrices() {
  m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
  m_viewMatrix = glm::lookAt(m_pos, m_pos + m_front, UP);
  m_VPMatrix = m_projectionMatrix * m_viewMatrix;
}

void FPSCamera::ProcessMouseMovement(float xOffset, float yOffset) {
  m_yaw += xOffset * m_mouseSensitivity;
  m_pitch -= yOffset * m_mouseSensitivity; // need to flip yOffset (screen space)
  m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
  glm::vec3 eulers;
  eulers.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  eulers.y = glm::sin(glm::radians(m_pitch));
  eulers.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  m_front = glm::normalize(eulers);
  m_right = glm::normalize(glm::cross(m_front, UP));
  UpdateMatrices();
}

void FPSCamera::OnImGui() {
  ImGui::Text("Position: %.1f, %.1f, %.1f", m_pos.x, m_pos.y, m_pos.z);
  ImGui::SliderFloat("Movement Speed",
                     &m_movementSpeed,
                     MIN_MOVE_SPEED,
                     MAX_MOVE_SPEED);
  ImGui::SliderFloat("Mouse Sensitivity",
                     &m_mouseSensitivity,
                     MIN_MOUSE_SENSITIVITY,
                     MAX_MOUSE_SENSITIVITY);
  if (ImGui::Button("Reset Settings")) {
    m_movementSpeed = DEFAULT_MOVEMENT_SPEED;
    m_mouseSensitivity = DEFAULT_MOUSE_SENSITIVITY;
  }
}
