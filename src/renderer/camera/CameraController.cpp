//
// Created by Tony Adriansen on 3/8/24.
//

#include "CameraController.hpp"
#include "src/core/Input.hpp"
#include "src/core/Logger.hpp"
#include <imgui/imgui.h>
#include <src/Common.hpp>

void CameraController::SetAspectRatio(float aspectRatio) {
  m_orbitCamera->SetAspectRatio(aspectRatio);
  m_fpsCamera->SetAspectRatio(aspectRatio);
}

CameraController::CameraController(const Window& window)
    : m_focused(false), m_mode(DEFAULT_MODE), m_activeCamera(nullptr), m_window(window) {
  m_orbitCamera = std::make_unique<OrbitCamera>(window.GetAspectRatio());
  m_fpsCamera = std::make_unique<FPSCamera>(window.GetAspectRatio());
  SetMode(DEFAULT_MODE);
}

void CameraController::SetMode(CameraController::Mode mode) {
  m_mode = mode;
  switch (m_mode) {
    case Mode::FPS:m_activeCamera = m_fpsCamera.get();
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
  if (m_mode == Mode::FPS && button == GLFW_MOUSE_BUTTON_LEFT && !m_focused) {
    m_focused = true;
    Input::CenterCursor();
    Input::SetCursorVisible(false);
  }
}

void CameraController::ProcessMouseMovement(double xOffset, double yOffset) {
  switch (m_mode) {
    case Mode::FPS:
      if (m_focused) {
        auto dimensions = m_window.GetWindowDimensions();
        auto cursorPos = Input::GetMousePosition();

        // Calculate the center of the window
        float centerX = dimensions.x / 2.0f;
        float centerY = dimensions.y / 2.0f;

        // Calculate the offset from the center of the window
        float offsetX = cursorPos.x - centerX;
        float offsetY = cursorPos.y - centerY;

        // Process mouse movement based on the offset from the center
        m_activeCamera->ProcessMouseMovement(offsetX, offsetY);
        // Center the cursor again
        Input::SetCursorPos(centerX, centerY);
      }
      break;
    case Mode::ORBIT:
      if (Input::IsMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
        m_activeCamera->ProcessMouseMovement(xOffset, yOffset);
      }
      break;
  }
}

void CameraController::Focus() {
  if (!m_focused) {
    m_focused = true;
    if (m_mode == Mode::FPS) {
      Input::SetCursorVisible(false);
      Input::CenterCursor();
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

Camera* CameraController::GetActiveCamera() const {
  return m_activeCamera;
}

void CameraController::OnMouseScrollEvent(double yOffset) {
  if (m_mode == Mode::FPS && m_focused) {
    m_activeCamera->OnMouseScrollEvent(yOffset);
  } else if (m_mode == Mode::ORBIT) {
    m_activeCamera->OnMouseScrollEvent(yOffset);
  }
}