//
// Created by Tony Adriansen on 3/1/24.
//

#include "FPSCamera.hpp"
#include "src/renderer/Input.hpp"
#include "src/renderer/Renderer.hpp"

void FPSCamera::SetPosition(const glm::vec3& newPos) {
  m_pos = newPos;
}

void FPSCamera::Update(double dt) {
  float offset = m_movementSpeed * (float) dt;
  bool dirty = false;
  if (Input::IsKeyDown(GLFW_KEY_W)) {
    m_pos += m_front * offset;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_S)) {
    m_pos -= m_front * offset;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_D)) {
    m_pos += glm::normalize(glm::cross(m_front, UP)) * offset;
    dirty = true;
  }
  if (Input::IsKeyDown(GLFW_KEY_A)) {
    m_pos -= glm::normalize(glm::cross(m_front, UP)) * offset;
    dirty = true;
  }
  if (dirty) UpdateMatrices();
}

FPSCamera::FPSCamera() :
    m_nearPlane(0.1),
    m_farPlane(1000),
    m_fov(45.0f),
    m_aspectRatio(Renderer::GetAspectRatio()),
    m_pos({0, 0, 3}),
    m_front({0, 0, -1}),
    m_yaw(-90.0f),
    m_pitch(0) {
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
