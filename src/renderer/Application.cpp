//
// Created by Tony Adriansen on 2/28/2024.
//

#include "Application.hpp"
#include "src/renderer/resource/ShaderManager.hpp"
#include "imgui/imgui.h"
#include "src/core/Logger.hpp"
#include "src/imgui/ImGuiMenu.hpp"
#include "src/Common.hpp"

#include "src/renderer/resource/MeshManager.hpp"
#include "src/renderer/shapes/Cube.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "src/renderer/ModelManager.hpp"

#include "src/scenes/LightingOneScene.hpp"
#include "src/scenes/PlaygroundScene.hpp"
#include "src/scenes/ModelViewerScene.hpp"

Application* Application::instancePtr = nullptr;

#define DEFAULT_RENDER_TO_IMGUI_VIEWPORT false

Application::Application()
    : m_cameraController(m_window), m_renderer(m_window) {
  instancePtr = this;
  Window::SetVsync(false);
  SetupResources();

  m_renderToImGuiViewport = DEFAULT_RENDER_TO_IMGUI_VIEWPORT;

  Input::Initialize(m_window.GetContext());

  m_cameraController.SetAspectRatio(m_window.GetAspectRatio());
  m_renderer.Init();

//  if (!m_renderToImGuiViewport) m_cameraController.Focus();
}

void Application::SetupResources() {
  LoadShaders();
//  ModelManager::LoadModel("backpack", "resources/models/backpack/backpack.obj");
//  ModelManager::LoadModel("teapot", "resources/models/teapot/teapot.obj");
//  ModelManager::LoadModel("sponza", "/Users/tony/Desktop/sponza/sponza.obj");
  ModelManager::LoadModel("spot", "resources/models/spot/spot_quadrangulated.obj");

  MeshManager::AddMesh("cube", Cube::Vertices, Cube::Indices);
  MeshManager::AddMesh("cube1024", Cube::Create(1024, 1024));

  std::vector<std::string> sky1Strings = {
      GET_TEXTURE_PATH("skybox1/px.png"),
      GET_TEXTURE_PATH("skybox1/nx.png"),
      GET_TEXTURE_PATH("skybox1/py.png"),
      GET_TEXTURE_PATH("skybox1/ny.png"),
      GET_TEXTURE_PATH("skybox1/pz.png"),
      GET_TEXTURE_PATH("skybox1/nz.png"),
  };

  std::vector<std::string> sky2Strings = {
      GET_TEXTURE_PATH("skybox2/right.jpg"),
      GET_TEXTURE_PATH("skybox2/left.jpg"),
      GET_TEXTURE_PATH("skybox2/top.jpg"),
      GET_TEXTURE_PATH("skybox2/bottom.jpg"),
      GET_TEXTURE_PATH("skybox2/front.jpg"),
      GET_TEXTURE_PATH("skybox2/back.jpg"),
  };
  std::vector<std::string> sky3Strings = {
      GET_TEXTURE_PATH("skybox_church/lposx.png"),
      GET_TEXTURE_PATH("skybox_church/lnegx.png"),
      GET_TEXTURE_PATH("skybox_church/lposy.png"),
      GET_TEXTURE_PATH("skybox_church/lnegy.png"),
      GET_TEXTURE_PATH("skybox_church/lposz.png"),
      GET_TEXTURE_PATH("skybox_church/lnegz.png"),
  };
  std::vector<std::string> sky4Strings = {
      GET_TEXTURE_PATH("skybox_winter/cposx.png"),
      GET_TEXTURE_PATH("skybox_winter/cnegx.png"),
      GET_TEXTURE_PATH("skybox_winter/cposy.png"),
      GET_TEXTURE_PATH("skybox_winter/cnegy.png"),
      GET_TEXTURE_PATH("skybox_winter/cposz.png"),
      GET_TEXTURE_PATH("skybox_winter/cnegz.png"),
  };

  TextureManager::AddTexture("Sky 1", sky1Strings);
  TextureManager::AddTexture("Sky 2", sky2Strings);
  TextureManager::AddTexture("Church", sky3Strings);
  TextureManager::AddTexture("Winter", sky4Strings);

  TextureManager::AddTexture("oak",
                             "resources/textures/oak_pbr/textures/oak_veneer_01_diff_1k.jpg",
                             Texture::SamplerType::TwoD);
  TextureManager::AddTexture("spot_texture", "resources/models/spot/spot_texture.png", Texture::SamplerType::TwoD);
  TextureManager::AddTexture("woodContainerDiffuse",
                             GET_TEXTURE_PATH("container_diffuse.png"),
                             Texture::SamplerType::TwoD);
  TextureManager::AddTexture("woodContainerSpecular",
                             GET_TEXTURE_PATH("container_specular.png"),
                             Texture::SamplerType::TwoD);
  TextureManager::AddTexture("woodContainerEmission",
                             GET_TEXTURE_PATH("container_emission.jpg"),
                             Texture::SamplerType::TwoD);

  std::vector<TexturePair> spotTextures = {
      {MatTextureType::Diffuse, TextureManager::GetTexture("oak")}
  };
  MaterialManager::AddMaterial("spotTextured", spotTextures, "blinnPhong");

  std::vector<TexturePair>
      woodContainerTextures = {{MatTextureType::Diffuse, TextureManager::GetTexture("woodContainerDiffuse")},
                               {MatTextureType::Specular, TextureManager::GetTexture("woodContainerDiffuse")}};
  MaterialManager::AddMaterial("woodContainer", woodContainerTextures, "blinnPhong");
}

void Application::Run() {
  m_renderer.SetSkyboxTexture(TextureManager::GetTexture(HashedString("Sky 2")));
//  m_sceneManager.AddScene("Playground", std::make_unique<PlaygroundScene>());
  m_sceneManager.AddScene("Lighting One", std::make_unique<LightingOneScene>());
  m_sceneManager.AddScene("Model Viewer", std::make_unique<ModelViewerScene>());

  m_sceneManager.SetActiveScene("Model Viewer");
//  m_sceneManager.SetActiveScene("Lighting One");
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

    m_sceneManager.GetActiveScene()->PreRender();

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

  ImGui::Begin("Main");

  static bool demoWindow = false;
  ImGui::Checkbox("Show Demo", &demoWindow);

  m_sceneManager.ImGuiSceneSelect();
  if (ImGui::CollapsingHeader("Metrics", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::Text("Draw Calls: %i", rendererStats.drawCalls);
    ImGui::Text("Vertices: %i", rendererStats.vertices);
    ImGui::Text("Indices: %i", rendererStats.indices);
    ImGui::Text("Shader Binds: %i", rendererStats.numShaderBinds);
    ImGui::Text("Material Swaps: %i", rendererStats.numMaterialSwitches);
  }

  if (ImGui::CollapsingHeader("Render Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
    constexpr std::array<const char*, 4> items = {"Default", "Normals", "Diffuse", "Depth Buffer"};
    int selectedItem = static_cast<int>(m_renderer.debugMode);
    if (ImGui::BeginCombo("##RenderModeCombo", ("Mode: " + std::string(items[selectedItem])).c_str())) {
      for (int i = 0; i < items.size(); i++) {
        const bool isSelected = (selectedItem == i);
        if (ImGui::Selectable(items[i], isSelected)) {
          selectedItem = i;
          m_renderer.debugMode = (Renderer::DebugMode) selectedItem;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::TreeNode("Maps")) {
      ImGui::Checkbox("Diffuse", &rendererSettings.diffuseMapEnabled);
      ImGui::SameLine();
      ImGui::Checkbox("Specular", &rendererSettings.specularMapEnabled);
      ImGui::Checkbox("Normal", &rendererSettings.normalMapEnabled);
      ImGui::SameLine();
      ImGui::Checkbox("Emission", &rendererSettings.emissionMapEnabled);
      ImGui::TreePop();
    }

    ImGui::Checkbox("MSAA", &rendererSettings.useMSAA);
    ImGui::Checkbox("Wireframe", &rendererSettings.wireframe);
    ImGui::Checkbox("Use Blinn", &rendererSettings.useBlinn);
    ImGui::Checkbox("Skybox", &rendererSettings.renderSkybox);
    ImGui::Checkbox("Render to ImGui Viewport", &m_renderToImGuiViewport);
    auto& ppSettings = m_renderer.ppUniforms;
    ImGui::Checkbox("Invert", &ppSettings.invert);
    ImGui::Checkbox("Flip", &ppSettings.flip);
    ImGui::DragFloat("Contrast", &m_renderer.ppUniforms.contrast, 0.01, -1, 1);

    if (ImGui::Button("Recompile Shaders")) {
      m_renderer.RecompileShaders();
    }

    static char screenshotBuffer[50];
    ImGui::SameLine();
    if (ImGui::Button("Screenshot")) {
      m_screenshotFilename = screenshotBuffer;
      if (m_screenshotFilename.empty()) {
        m_renderer.Screenshot();
      } else {
        m_renderer.Screenshot(m_screenshotFilename);
      }
    }
    ImGui::InputTextWithHint("##screenshotFilename", "Screenshot Filename", screenshotBuffer, sizeof(screenshotBuffer));
  }

  if (ImGui::CollapsingHeader("Camera")) {
    m_cameraController.OnImGui();
  }

  if (ImGui::CollapsingHeader("Lights")) {
    ImGui::Text("Enabled Lights");
    ImGui::Checkbox("Directional", &rendererSettings.renderDirectionalLights);
    ImGui::SameLine();
    ImGui::Checkbox("Point", &rendererSettings.renderPointLights);
    ImGui::SameLine();
    ImGui::Checkbox("Spot", &rendererSettings.renderSpotlights);
    m_sceneManager.GetActiveScene()->ImGuiLights();
  }

  ImGui::End();

  m_sceneManager.GetActiveScene()->OnImGui();

  if (demoWindow)
    ImGui::ShowDemoWindow(&demoWindow);

  // need to set focus on window if user is aiming camera in it
  static bool imguiViewportFocused = false;
  static bool isFirstMouse = true;
  static ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoNav;

  if (m_renderToImGuiViewport) {
    bool viewportOpen = true;
    ImGui::Begin("Viewport", &viewportOpen, viewportFlags);
    imguiViewportFocused = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
    // ImGui flips UV coords, need to swap
    ImGui::Image((void*) (intptr_t) m_renderer.GetFinalImageTexture().Id(),
                 ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

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
  m_renderer.ResizeViewport(width, height);
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
  m_cameraController.SetMode(activeScene->defaultCameraMode);
  m_cameraController.GetActiveCamera()->SetPosition(activeScene->defaultCameraPosition);
  m_renderer.SetDirectionalLight(activeScene->GetDirectionalLight());
  m_renderer.SetPointLights(activeScene->GetPointLights());
  m_renderer.SetSpotLights(activeScene->GetSpotLights());
//        m_cameraController.GetActiveCamera().SetTargetPos({0, 0, 0});
}

void Application::OnKeyEvent(int key, int action, int mods) {
  bool pressed = action == GLFW_PRESS;
  if (!pressed) return;

  if (key == GLFW_KEY_BACKSPACE && mods == GLFW_MOD_SHIFT) {
    m_window.SetShouldClose(true);
  }

  if (ImGui::GetIO().WantCaptureKeyboard) return;

  if (key == GLFW_KEY_M) {
    m_renderer.RecompileShaders();
  } else if (key == GLFW_KEY_N) {
    m_settings.showImGui = !m_settings.showImGui;
    if (!m_settings.showImGui) m_renderToImGuiViewport = false;
  } else if (key == GLFW_KEY_C) {
    m_renderer.Screenshot();
  }
}

void Application::LoadShaders() {
  ShaderManager::AddShader("default", {{GET_SHADER_PATH("default.vert"), ShaderType::VERTEX},
                                       {GET_SHADER_PATH("default.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("blinnPhong", {{GET_SHADER_PATH("blinnPhong.vert"), ShaderType::VERTEX},
                                          {GET_SHADER_PATH("blinnPhong.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("skybox", {{GET_SHADER_PATH("skybox.vert"), ShaderType::VERTEX},
                                      {GET_SHADER_PATH("skybox.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("singleColor", {{GET_SHADER_PATH("singleColor.vert"), ShaderType::VERTEX},
                                           {GET_SHADER_PATH("singleColor.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("contrast", {{GET_SHADER_PATH("contrast.vert"), ShaderType::VERTEX},
                                        {GET_SHADER_PATH("contrast.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("invert", {{GET_SHADER_PATH("postprocessing/invert.vert"), ShaderType::VERTEX},
                                      {GET_SHADER_PATH("postprocessing/invert.frag"), ShaderType::FRAGMENT}});
}
