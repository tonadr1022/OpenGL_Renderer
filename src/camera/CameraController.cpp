//
// Created by Tony Adriansen on 3/8/24.
//

#include "CameraController.hpp"

#include <imgui/imgui.h>

#include <src/Common.hpp>
#include <string>

#include "src/utils/Input.hpp"

void CameraController::SetAspectRatio(float aspectRatio) {
  m_orbitCamera->SetAspectRatio(aspectRatio);
  m_fpsCamera->SetAspectRatio(aspectRatio);
}

CameraController::CameraController(const Window &window)
  : m_focused(false), m_mode(DefaultMode), m_activeCamera(nullptr), m_window(window) {
  m_orbitCamera = std::make_unique<OrbitCamera>(window.GetAspectRatio());
  m_fpsCamera = std::make_unique<FPSCamera>(window.GetAspectRatio());
  SetMode(DefaultMode);
}

void CameraController::SetMode(CameraController::Mode mode) {
  m_mode = mode;
  switch (m_mode) {
    case Mode::FPS:
      m_activeCamera = m_fpsCamera.get();
      break;
    case Mode::Orbit:
      m_activeCamera = m_orbitCamera.get();
      break;
  }
}

void CameraController::Update(double dt) {
  if (Input::IsKeyPressed(GLFW_KEY_TAB) || Input::IsKeyPressed(GLFW_KEY_BACKSLASH) ||
    Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
    if (m_focused) {
      Unfocus();
    } else {
      Focus();
    }
  }
  m_activeCamera->Update(dt);
}

void CameraController::OnImGui() {
  if (ImGui::BeginCombo("##cameramode",
                        ("Mode: " + std::string(ModeToString[static_cast<int>(m_mode)])).c_str())) {
    if (ImGui::Selectable("FPS", m_mode == Mode::FPS)) {
      SetMode(Mode::FPS);
    }
    if (ImGui::Selectable("Orbit", m_mode == Mode::Orbit)) {
      SetMode(Mode::Orbit);
    }
    ImGui::EndCombo();
  }
  m_activeCamera->OnImGui();
}

void CameraController::OnMouseButtonEvent(int button, int /*action*/) {
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
        auto cursor_pos = Input::GetMousePosition();

        // Calculate the center of the window
        float center_x = static_cast<float>(dimensions.x) / 2.0f;
        float center_y = static_cast<float>(dimensions.y) / 2.0f;

        // Calculate the offset from the center of the window
        float offset_x = cursor_pos.x - center_x;
        float offset_y = cursor_pos.y - center_y;

        // Process mouse movement based on the offset from the center
        m_activeCamera->ProcessMouseMovement(offset_x, offset_y);
        // Center the cursor again
        Input::SetCursorPos(center_x, center_y);
      }
      break;
    case Mode::Orbit:
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
      // Input::SetCursorVisible(false);
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

Camera *CameraController::GetActiveCamera() const { return m_activeCamera; }

void CameraController::OnMouseScrollEvent(double yOffset) {
  if (m_mode == Mode::FPS && m_focused || m_mode == Mode::Orbit) {
    m_activeCamera->OnMouseScrollEvent(yOffset);
  }
}
