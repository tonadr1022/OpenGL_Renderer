//
// Created by Tony Adriansen on 3/5/24.
//

#include "Camera.hpp"

Camera::Camera(float aspectRatio)
    : m_pos({0, 0, 3}),
      m_aspectRatio(aspectRatio),
      m_nearPlane(0.1),
      m_farPlane(1000) {}

void Camera::SetAspectRatio(float aspectRatio) {
  m_aspectRatio = aspectRatio;
}

void Camera::SetPosition(const glm::vec3& newPos) {
  m_pos = newPos;
}

void Camera::Update(double dt) {}

