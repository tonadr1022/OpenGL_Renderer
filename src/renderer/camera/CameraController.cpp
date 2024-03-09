//
// Created by Tony Adriansen on 3/8/24.
//

#include "CameraController.hpp"
#include "src/renderer/Input.hpp"
#include <imgui/imgui.h>
#include <src/Common.hpp>

void CameraController::SetAspectRatio(float aspectRatio) {
  m_orbitCamera->SetAspectRatio(aspectRatio);
  m_fpsCamera->SetAspectRatio(aspectRatio);
}

CameraController::CameraController(float aspectRatio)
    : m_focused(false), m_mode(DEFAULT_MODE), m_activeCamera(nullptr) {
  m_orbitCamera = std::make_unique<OrbitCamera>(aspectRatio);
  m_fpsCamera = std::make_unique<FPSCamera>(aspectRatio);
  SetMode(DEFAULT_MODE);
}

void CameraController::SetMode(CameraController::Mode mode) {
  m_mode = mode;
  switch (m_mode) {
    case Mode::FPS: m_activeCamera = m_fpsCamera.get();
      break;
    case Mode::ORBIT: m_activeCamera = m_orbitCamera.get();
      break;
  }
}

void CameraController::Update(double dt) {
  if (Input::IsKeyPressed(GLFW_KEY_TAB) || Input::IsKeyPressed(GLFW_KEY_BACKSLASH)
      || Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
    if (m_focused) {
      Unfocus();
    } else {
      Focus();
    }
  }
  m_activeCamera->Update(dt);
}

void CameraController::OnImGui() {
  ImGui::Begin("Camera");
  if (ImGui::Button("FPS")) {
    SetMode(CameraController::Mode::FPS);
  }
  ImGui::SameLine();
  if (ImGui::Button("Orbit")) {
    SetMode(CameraController::Mode::ORBIT);
  }
  ImGui::Text("Mode: %s", m_mode == Mode::FPS ? "FPS" : "ORBIT");
  m_activeCamera->OnImGui();
  ImGui::End();
}

void CameraController::OnMouseButtonEvent(int button, int action) {
//  if (m_mode == Mode::FPS && button == GLFW_MOUSE_BUTTON_LEFT && !m_focused) {
//    m_focused = true;
//    Input::SetCursorVisible(false);
//  }
}

void CameraController::ProcessMouseMovement(double xOffset, double yOffset) {
  switch (m_mode) {
    case Mode::FPS:if (m_focused) {
      m_activeCamera->ProcessMouseMovement(xOffset, yOffset);
    }
      break;
    case Mode::ORBIT:
      if (Input::IsMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
        m_activeCamera->ProcessMouseMovement(xOffset, yOffset);
      }
      break;
  }

  // TODO switch statement only update one

}

void CameraController::Focus() {
  if (!m_focused) {
    m_focused = true;
    if (m_mode == Mode::FPS) {
      Input::SetCursorVisible(false);
    }
  }
}

void CameraController::Unfocus() {
  m_focused = false;
  if (m_mode == Mode::FPS) {
    Input::SetCursorVisible(true);
    Input::CenterCursor();
  }
}

Camera& CameraController::GetActiveCamera() const {
  return *m_activeCamera;
}

void CameraController::OnMouseScrollEvent(double yOffset) {
  if (m_mode == Mode::FPS && m_focused) {
    m_activeCamera->OnMouseScrollEvent(yOffset);
  } else if (m_mode == Mode::ORBIT) {
    m_activeCamera->OnMouseScrollEvent(yOffset);
  }
}
