//
// Created by Tony Adriansen on 2/28/2024.
//

#include "Application.hpp"
#include "src/renderer/resource/ShaderManager.hpp"
#include "src/scenes/PlaygroundScene.hpp"
#include "imgui/imgui.h"
#include "src/core/Logger.hpp"
#include "src/imgui/ImGuiMenu.hpp"
#include "src/Common.hpp"

Application* Application::instancePtr = nullptr;

#define INIT_IMGUIRENDER false

Application::Application()
    : m_renderer(m_window, INIT_IMGUIRENDER), m_cameraMode(CameraMode::FPS), m_focused(false) {
  instancePtr = this;
  Window::SetVsync(false);

  m_renderToImGuiViewport = INIT_IMGUIRENDER;
  m_renderer.SetWindowSize(m_window.GetWidth(), m_window.GetHeight());
  auto& renderSettings = m_renderer.GetSettings();
  renderSettings.renderToImGuiViewport = m_renderToImGuiViewport;

  Input::Initialize(m_window.GetContext());

  m_fpsCamera = std::make_unique<FPSCamera>(m_window.GetAspectRatio());
  float aspectRatio = static_cast<float>(m_window.GetWidth()) / static_cast<float>(m_window.GetHeight());
  m_fpsCamera->SetAspectRatio(aspectRatio);
}

void Application::SetupResources() {
  ShaderManager::AddShader("default", {{GET_SHADER_PATH("default.vert"), ShaderType::VERTEX},
                                       {GET_SHADER_PATH("default.frag"), ShaderType::FRAGMENT}});
}

void Application::Run() {
  SetupResources();
  m_renderer.Init();

  m_sceneManager.AddScene(std::make_unique<PlaygroundScene>());
  m_sceneManager.SetActiveScene("Playground");
  m_renderer.SetActiveCamera(m_fpsCamera.get());
  double currTime, lastTime = glfwGetTime(), deltaTime;
  while (!m_window.ShouldClose()) {
    currTime = glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;

    // input
    Input::Update();
    if (Input::IsKeyPressed(GLFW_KEY_TAB) || Input::IsKeyPressed(GLFW_KEY_BACKSLASH)
        || Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
      if (m_focused) {
        m_focused = false;
        Input::SetCursorVisible(true);
      } else {
        m_focused = true;
        Input::SetCursorVisible(false);
      }
    }


    // update
    m_sceneManager.GetActiveScene()->Update(deltaTime);
    m_fpsCamera->Update(deltaTime);

    // render
    ImGuiMenu::StartFrame(m_renderToImGuiViewport);

    m_renderer.RenderScene(*m_sceneManager.GetActiveScene());
    DisplayImGui();

    ImGuiMenu::EndFrame();
    m_window.SwapBuffers();
  }
}

void Application::DisplayImGui() {
  auto& rendererStats = m_renderer.GetStats();
  auto& rendererSettings = m_renderer.GetSettings();

  ImGui::Begin("Metrics");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
  ImGui::Text("Draw Calls: %i", rendererStats.drawCalls);
  ImGui::Text("Vertices: %i", rendererStats.vertices);
  ImGui::Text("Indices: %i", rendererStats.indices);
  ImGui::End(); // metrics

  ImGui::Begin("Settings");
  ImGui::Checkbox("Wireframe", &rendererSettings.wireframe);
  if (ImGui::Checkbox("Render to ImGui Viewport", &m_renderToImGuiViewport)) {
    rendererSettings.renderToImGuiViewport = m_renderToImGuiViewport;
  }
  ImGui::End(); // settings

  ImGui::Begin("Camera");
  m_fpsCamera->OnImGui();
  ImGui::End();


  // need to set focus on window if user is aiming camera in it
  if (m_renderToImGuiViewport) {
    ImGui::Begin("Viewport");
    // ImGui flips UV coords, need to swap
    ImGui::Image((void*) (intptr_t) m_renderer.GetFrameCapturer().GetTexture().Id(),
                 ImGui::GetContentRegionAvail(),
                 ImVec2(0, 1),
                 ImVec2(1, 0));

    // handle window resize
    static ImVec2 prevDimensions{0, 0};
    ImVec2 currDimensions = ImGui::GetWindowSize();
    if (prevDimensions.x != currDimensions.x || prevDimensions.y != currDimensions.y) {
      // update cameras
      m_fpsCamera->SetAspectRatio(static_cast<float>(currDimensions.x) / static_cast<float>(currDimensions.y));
    }
    prevDimensions = currDimensions;

    // focus viewport window
    if (!m_focused && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      m_focused = true;
      Input::SetCursorVisible(false);
    }
    ImGui::End();
  }
}

void Application::OnViewportResize(uint32_t width, uint32_t height) {
  m_renderer.SetWindowSize(width, height);

  float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  m_fpsCamera->SetAspectRatio(aspectRatio);
}

void Application::OnMousePosMove(float xpos, float ypos) {
  static bool isFirstMouse = true;
  static float lastX, lastY;
  if (isFirstMouse) {
    lastX = xpos;
    lastY = ypos;
    isFirstMouse = false;
  }

  float xOffset = xpos - lastX;
  float yOffset = ypos - lastY;

  lastX = xpos;
  lastY = ypos;

  if (m_focused) {
    m_fpsCamera->ProcessMouseMovement(xOffset, yOffset);
  }
}
void Application::OnMouseButtonEvent(int button, int action) {
  if (ImGui::GetIO().WantCaptureMouse) return;
  if (m_cameraMode == CameraMode::FPS && button == GLFW_MOUSE_BUTTON_LEFT && !m_focused) {
    m_focused = true;
    Input::SetCursorVisible(false);
  }

}