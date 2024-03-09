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
#include "src/scenes/LightingOneScene.hpp"

Application* Application::instancePtr = nullptr;

#define INIT_IMGUIRENDER true

Application::Application()
    : m_cameraController(m_window.GetAspectRatio()),
      m_renderer(m_window, m_cameraController.GetActiveCamera(), INIT_IMGUIRENDER) {
  instancePtr = this;
  Window::SetVsync(false);
  SetupResources();

  m_renderToImGuiViewport = INIT_IMGUIRENDER;
  m_renderer.SetWindowSize(m_window.GetWidth(), m_window.GetHeight());
  auto& renderSettings = m_renderer.GetSettings();
  renderSettings.renderToImGuiViewport = m_renderToImGuiViewport;

  Input::Initialize(m_window.GetContext());
  m_cameraController.SetAspectRatio(m_window.GetAspectRatio());

  m_renderer.Init();
}

void Application::SetupResources() {
  ShaderManager::AddShader("default", {{GET_SHADER_PATH("default.vert"), ShaderType::VERTEX},
                                       {GET_SHADER_PATH("default.frag"), ShaderType::FRAGMENT}});
}

void Application::Run() {
  m_sceneManager.AddScene(std::make_unique<PlaygroundScene>());

m_sceneManager.AddScene(std::make_unique<LightingOneScene>());
  m_sceneManager.SetActiveScene("Playground");

  double currTime, lastTime = glfwGetTime(), deltaTime;
  while (!m_window.ShouldClose()) {
    currTime = glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;

    // input
    Input::Update();

    // update
    m_sceneManager.GetActiveScene()->Update(deltaTime);
    m_cameraController.Update(deltaTime);

    // render
    ImGuiMenu::StartFrame(m_renderToImGuiViewport);
    m_renderer.RenderScene(*m_sceneManager.GetActiveScene(), m_cameraController.GetActiveCamera());
    OnImGui();

    ImGuiMenu::EndFrame();
    m_window.SwapBuffers();
  }
}

void Application::OnImGui() {
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

  ImGui::Begin("Main");
  auto& names = m_sceneManager.GetSceneNames();
  if (ImGui::BeginMenu("Scenes")) {
    for (auto& name : names) {
      if (ImGui::MenuItem(name.data(), nullptr, false)) {
        m_sceneManager.SetActiveScene(name);
        m_cameraController.GetActiveCamera().SetPosition({0,2,-3});
        m_cameraController.GetActiveCamera().SetTargetPos({0,0,0});
      }
    }
    ImGui::EndMenu();
  }
  ImGui::End();

  m_cameraController.OnImGui();

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
      float aspectRatio = static_cast<float>(currDimensions.x) / static_cast<float>(currDimensions.y);
      m_cameraController.SetAspectRatio(aspectRatio);
    }
    prevDimensions = currDimensions;

    // focus viewport window
    if (!m_cameraController.IsFocused() && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      m_cameraController.Focus();
    }
    ImGui::End();
  }
}

void Application::OnViewportResize(uint32_t width, uint32_t height) {
  m_renderer.SetWindowSize(width, height);
  float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  m_cameraController.SetAspectRatio(aspectRatio);
}

void Application::OnMousePosMove(double xpos, double ypos) {
  if (!m_renderToImGuiViewport && ImGui::GetIO().WantCaptureMouse) return;
  static bool isFirstMouse = true;
  static double lastX, lastY;
  if (isFirstMouse) {
    lastX = xpos;
    lastY = ypos;
    isFirstMouse = false;
  }

  double xOffset = xpos - lastX;
  double yOffset = ypos - lastY;

  lastX = xpos;
  lastY = ypos;

  m_cameraController.ProcessMouseMovement(xOffset, yOffset);
}

void Application::OnMouseButtonEvent(int button, int action) {
  if (!m_renderToImGuiViewport && ImGui::GetIO().WantCaptureMouse) return;
  m_cameraController.OnMouseButtonEvent(button, action);
}

void Application::OnMouseScrollEvent(double yOffset) {
  if (!m_renderToImGuiViewport && ImGui::GetIO().WantCaptureMouse) return;
  m_cameraController.OnMouseScrollEvent(yOffset);
}