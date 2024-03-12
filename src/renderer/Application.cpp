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
#include "src/renderer/resource/MeshManager.hpp"
#include "src/renderer/shapes/Cube.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/renderer/resource/MaterialManager.hpp"

Application* Application::instancePtr = nullptr;

#define INIT_IMGUIRENDER false

Application::Application()
    : m_cameraController(m_window), m_renderer(m_window, INIT_IMGUIRENDER) {
  instancePtr = this;
  Window::SetVsync(false);
  SetupResources();

  m_renderToImGuiViewport = INIT_IMGUIRENDER;
  auto frameBufferDimensions = m_window.GetFrameBufferDimensions();
  m_renderer.SetFrameBufferSize(frameBufferDimensions.x, frameBufferDimensions.y);
  auto& renderSettings = m_renderer.GetSettings();
  renderSettings.renderToImGuiViewport = m_renderToImGuiViewport;

  Input::Initialize(m_window.GetContext());

  m_cameraController.SetAspectRatio(m_window.GetAspectRatio());
  m_renderer.Init();

//  if (!m_renderToImGuiViewport) m_cameraController.Focus();
}

void Application::SetupResources() {
  ShaderManager::AddShader("default", {{GET_SHADER_PATH("default.vert"), ShaderType::VERTEX},
                                       {GET_SHADER_PATH("default.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("blinnPhong", {{GET_SHADER_PATH("blinnPhong.vert"), ShaderType::VERTEX},
                                          {GET_SHADER_PATH("blinnPhong.frag"), ShaderType::FRAGMENT}});

  MeshManager::AddMesh("cube", Cube::Vertices, Cube::Indices);
  MeshManager::AddMesh("cube1024", Cube::Create(1024, 1024));
  TextureManager::AddTexture("cow", GET_TEXTURE_PATH("cow.png"), Texture::Type::Diffuse);

  std::vector<Texture*> textures = {TextureManager::AddTexture("woodContainerDiffuse",
                                                               GET_TEXTURE_PATH("container_diffuse.png"),
                                                               Texture::Type::Diffuse),
                                    TextureManager::AddTexture("woodContainerSpecular",
                                                               GET_TEXTURE_PATH("container_specular.png"),
                                                               Texture::Type::Specular)};
//                                    TextureManager::AddTexture("woodContainerEmission", GET_TEXTURE_PATH("container_emission.jpg"), Texture::Type::Emission)};
  MaterialManager::AddMaterial("woodContainer", textures, "blinnPhong");
}

void Application::Run() {
  m_sceneManager.AddScene(std::make_unique<PlaygroundScene>());
  m_sceneManager.AddScene(std::make_unique<LightingOneScene>());
  m_sceneManager.SetActiveScene("Lighting One");
  OnSceneChange();

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

    if (m_settings.showImGui) ImGuiMenu::StartFrame(m_renderToImGuiViewport);
    m_renderer.RenderScene(*m_sceneManager.GetActiveScene(), m_cameraController.GetActiveCamera());
    if (m_settings.showImGui) OnImGui();

    if (m_settings.showImGui) ImGuiMenu::EndFrame();
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

  ImGui::Text("Mode");
  if (ImGui::Button("Normals")) {
    m_renderer.debugMode = Renderer::DebugMode::Normals;
  }
  ImGui::SameLine();
  if (ImGui::Button("None")) {
    m_renderer.debugMode = Renderer::DebugMode::None;
  }
  if (ImGui::Button("Recompile Shaders")) {
    ShaderManager::RecompileShaders();
  }

  auto& names = m_sceneManager.GetSceneNames();
  if (ImGui::BeginMenu("Scenes")) {
    for (auto& name : names) {
      if (ImGui::MenuItem(name.data(), nullptr, false)) {
        m_sceneManager.SetActiveScene(name);
        OnSceneChange();
      }
    }
    ImGui::EndMenu();
  }


  ImGui::End();

  ImGui::Begin("Lights");
  ImGui::Text("Enabled Lights");
  ImGui::Checkbox("Directional", &rendererSettings.renderDirectionalLights);
  ImGui::SameLine();
  ImGui::Checkbox("Point", &rendererSettings.renderPointLights);
  ImGui::SameLine();
  ImGui::Checkbox("Spot", &rendererSettings.renderSpotlights);

  m_sceneManager.GetActiveScene()->ImGuiLights();
  ImGui::End();

  m_cameraController.OnImGui();


  // need to set focus on window if user is aiming camera in it
  static bool imguiViewportFocused = false;
  static bool isFirstMouse = true;
  if (m_renderToImGuiViewport) {
    ImGui::Begin("Viewport");
    imguiViewportFocused = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
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
    if (imguiViewportFocused) {
      auto mouseOffset = Input::GetMousePosOffset();
      if (isFirstMouse) {
        mouseOffset = {0.0f, 0.0f};
        isFirstMouse = false;
      }
      if (mouseOffset != glm::vec2(0.0f)) {
        m_cameraController.ProcessMouseMovement(mouseOffset.x, mouseOffset.y);
      }
    } else {
      isFirstMouse = true;
    }
    ImGui::End();
  }
}

void Application::OnViewportResize(uint32_t width, uint32_t height) {
  m_renderer.SetFrameBufferSize(width, height);
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

  if (!m_renderToImGuiViewport) {
    m_cameraController.ProcessMouseMovement(xOffset, yOffset);
  }
}

void Application::OnMouseButtonEvent(int button, int action) {
  if (!m_renderToImGuiViewport && ImGui::GetIO().WantCaptureMouse) return;
  m_cameraController.OnMouseButtonEvent(button, action);
}

void Application::OnMouseScrollEvent(double yOffset) {
  if (!m_renderToImGuiViewport && ImGui::GetIO().WantCaptureMouse) return;
  m_cameraController.OnMouseScrollEvent(yOffset);
}

void Application::OnSceneChange() {
  Scene* activeScene = m_sceneManager.GetActiveScene();
  m_cameraController.GetActiveCamera()->SetPosition(activeScene->defaultCameraPosition);
  m_renderer.SetDirectionalLight(activeScene->GetDirectionalLight());
  m_renderer.SetPointLights(activeScene->GetPointLights());
  m_renderer.SetSpotLights(activeScene->GetSpotLights());
//        m_cameraController.GetActiveCamera().SetTargetPos({0, 0, 0});
}

void Application::OnKeyEvent(int key, int action) {
  bool pressed = action == GLFW_PRESS;
  if (pressed) {
    if (key == GLFW_KEY_BACKSPACE) {
      m_window.SetShouldClose(true);
    } else if (key == GLFW_KEY_M) {
      ShaderManager::RecompileShaders();
    } else if (key == GLFW_KEY_N) {
      m_settings.showImGui = !m_settings.showImGui;
      if (!m_settings.showImGui) m_renderToImGuiViewport = false;
    }
  }
}
