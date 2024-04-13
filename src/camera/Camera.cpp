//
// Created by Tony Adriansen on 3/5/24.
//

#include "Camera.hpp"

Camera::Camera(float aspectRatio)
    : m_pos({10, 10, 10}), m_aspectRatio(aspectRatio), m_nearPlane(0.1), m_farPlane(1000) {}

void Camera::SetAspectRatio(float aspectRatio) {
  m_aspectRatio = aspectRatio;
  m_dirty = true;
}

void Camera::SetPosition(const glm::vec3& newPos) {
  m_pos = newPos;
  m_dirty = true;
}

void Camera::Update(double dt) {}
