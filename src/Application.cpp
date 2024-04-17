//
// Created by Tony Adriansen on 2/28/2024.
//

#include "Application.hpp"

#include <cstdint>

#include "imgui/imgui.h"
#include "src/Common.hpp"
#include "src/imgui/ImGuiMenu.hpp"
#include "src/resource/MaterialManager.hpp"
#include "src/resource/MeshManager.hpp"
#include "src/resource/ModelManager.hpp"
#include "src/resource/ShaderManager.hpp"
#include "src/resource/TextureManager.hpp"
#include "src/scenes/InstancingScene.hpp"
#include "src/scenes/LightingOneScene.hpp"
#include "src/scenes/ModelViewerScene.hpp"
#include "src/scenes/PlaygroundScene.hpp"
#include "src/shapes/Cube.hpp"
#include "src/utils/Input.hpp"
#include "src/utils/Logger.hpp"

using ImGui::GetContentRegionAvail;

Application *Application::m_instance_ptr = nullptr;

#define DEFAULT_RENDER_TO_IMGUI_VIEWPORT false
#define DEFAULT_VSYNC true

Application::Application()
    : m_cameraController(m_window),
      m_renderer(m_window),
      m_sceneManager(m_renderer, m_cameraController) {
  m_instance_ptr = this;
  m_window.SetVsync(DEFAULT_VSYNC);
  SetupResources();

  m_renderToImGuiViewport = DEFAULT_RENDER_TO_IMGUI_VIEWPORT;

  Input::Initialize(m_window.GetContext());

  m_cameraController.SetAspectRatio(m_window.GetAspectRatio());
  m_renderer.Init();

  if (!m_renderToImGuiViewport) m_cameraController.Focus();
}

void Application::SetupResources() {
  LoadShaders();
  ModelManager::LoadModel("backpack", "resources/models/backpack/backpack.obj");
  ModelManager::LoadModel("teapot", "resources/models/teapot/teapot.obj");
  ModelManager::LoadModel("sponza", "resources/models/sponza/sponza.obj");
  ModelManager::LoadModel("spot", "resources/models/spot/spot_quadrangulated.obj");

  MeshManager::AddMesh("cube", Cube::Vertices, Cube::Indices);
  MeshManager::AddMesh("cube1024", Cube::Create(1024, 1024));

  std::vector<std::string> sky1_strings = {
      GET_TEXTURE_PATH("skybox1/px.png"), GET_TEXTURE_PATH("skybox1/nx.png"),
      GET_TEXTURE_PATH("skybox1/py.png"), GET_TEXTURE_PATH("skybox1/ny.png"),
      GET_TEXTURE_PATH("skybox1/pz.png"), GET_TEXTURE_PATH("skybox1/nz.png"),
  };

  std::vector<std::string> sky2_strings = {
      GET_TEXTURE_PATH("skybox2/right.jpg"), GET_TEXTURE_PATH("skybox2/left.jpg"),
      GET_TEXTURE_PATH("skybox2/top.jpg"),   GET_TEXTURE_PATH("skybox2/bottom.jpg"),
      GET_TEXTURE_PATH("skybox2/front.jpg"), GET_TEXTURE_PATH("skybox2/back.jpg"),
  };
  std::vector<std::string> sky3_strings = {
      GET_TEXTURE_PATH("skybox_church/lposx.png"), GET_TEXTURE_PATH("skybox_church/lnegx.png"),
      GET_TEXTURE_PATH("skybox_church/lposy.png"), GET_TEXTURE_PATH("skybox_church/lnegy.png"),
      GET_TEXTURE_PATH("skybox_church/lposz.png"), GET_TEXTURE_PATH("skybox_church/lnegz.png"),
  };
  std::vector<std::string> sky4_strings = {
      GET_TEXTURE_PATH("skybox_winter/cposx.png"), GET_TEXTURE_PATH("skybox_winter/cnegx.png"),
      GET_TEXTURE_PATH("skybox_winter/cposy.png"), GET_TEXTURE_PATH("skybox_winter/cnegy.png"),
      GET_TEXTURE_PATH("skybox_winter/cposz.png"), GET_TEXTURE_PATH("skybox_winter/cnegz.png"),
  };

  TextureManager::AddTexture("Sky 1", sky1_strings);
  TextureManager::AddTexture("Sky 2", sky2_strings);
  TextureManager::AddTexture("Church", sky3_strings);
  TextureManager::AddTexture("Winter", sky4_strings);

  TextureManager::AddTexture("oak", "resources/textures/oak_pbr/textures/oak_veneer_01_diff_1k.jpg",
                             Texture::SamplerType::TwoD);
  TextureManager::AddTexture("spot_texture", "resources/models/spot/spot_texture.png",
                             Texture::SamplerType::TwoD);
  TextureManager::AddTexture("woodContainerDiffuse", GET_TEXTURE_PATH("container_diffuse.png"),
                             Texture::SamplerType::TwoD);
  TextureManager::AddTexture("woodContainerSpecular", GET_TEXTURE_PATH("container_specular.png"),
                             Texture::SamplerType::TwoD);
  TextureManager::AddTexture("woodContainerEmission", GET_TEXTURE_PATH("container_emission.jpg"),
                             Texture::SamplerType::TwoD);

  std::vector<TexturePair> spot_textures = {
      {MatTextureType::Diffuse, TextureManager::GetTexture("spot_texture")}};
  MaterialManager::AddMaterial("spotTextured", spot_textures, "blinnPhong");

  std::vector<TexturePair> wood_container_textures = {
      {MatTextureType::Diffuse, TextureManager::GetTexture("woodContainerDiffuse")},
      {MatTextureType::Specular, TextureManager::GetTexture("woodContainerDiffuse")}};
  MaterialManager::AddMaterial("woodContainer", wood_container_textures, "blinnPhong");
}

void Application::Run() {
  m_sceneManager.AddScene("Playground", std::make_unique<PlaygroundScene>());
  m_sceneManager.AddScene("Lighting One", std::make_unique<LightingOneScene>());
  m_sceneManager.AddScene("Model Viewer", std::make_unique<ModelViewerScene>());
  m_sceneManager.AddScene("Instancing 1", std::make_unique<InstancingScene>());

  m_sceneManager.SetActiveScene("Instancing 1");

  double curr_time;
  double last_time = glfwGetTime();
  double delta_time;
  while (!m_window.ShouldClose()) {
    curr_time = glfwGetTime();
    delta_time = curr_time - last_time;
    last_time = curr_time;
    // input
    Input::Update();
    // update
    m_sceneManager.GetActiveScene()->Update(delta_time);
    m_cameraController.Update(delta_time);
    m_sceneManager.GetActiveScene()->PreRender();

    // render
    if (m_settings.showImGui) ImGuiMenu::StartFrame(m_renderToImGuiViewport);
    m_renderer.RenderScene(*m_sceneManager.GetActiveScene(), m_cameraController.GetActiveCamera());
    if (m_settings.showImGui) OnImGui();
    if (m_settings.showImGui) ImGuiMenu::EndFrame();
    m_window.SwapBuffers();
    GL_LOG_ERROR();
  }
}

void Application::OnImGui() {
  const auto &renderer_stats = m_renderer.GetStats();
  auto &renderer_settings = m_renderer.GetSettings();

  ImGui::Begin("Main");

  static bool demo_window = false;
  ImGui::Checkbox("Show Demo", &demo_window);

  m_sceneManager.ImGuiSceneSelect();
  if (ImGui::CollapsingHeader("Metrics", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::Text("Draw Calls: %i", renderer_stats.drawCalls);
    ImGui::Text("Vertices: %i", renderer_stats.vertices);
    ImGui::Text("Indices: %i", renderer_stats.indices);
    ImGui::Text("Shader Binds: %i", renderer_stats.numShaderBinds);
    ImGui::Text("Material Swaps: %i", renderer_stats.numMaterialSwitches);
  }

  if (ImGui::CollapsingHeader("Render Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
    constexpr std::array<const char *, 4> Items = {"Default", "Normals", "Diffuse", "Depth Buffer"};
    int selected_item = static_cast<int>(m_renderer.debugMode);
    if (ImGui::BeginCombo("##RenderModeCombo",
                          ("Mode: " + std::string(Items[selected_item])).c_str())) {
      for (int i = 0; i < Items.size(); i++) {
        const bool is_selected = (selected_item == i);
        if (ImGui::Selectable(Items[i], is_selected)) {
          selected_item = i;
          m_renderer.debugMode = static_cast<Renderer::DebugMode>(selected_item);
          if (is_selected) ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::TreeNode("Maps")) {
      ImGui::Checkbox("Diffuse", &renderer_settings.diffuseMapEnabled);
      ImGui::SameLine();
      ImGui::Checkbox("Specular", &renderer_settings.specularMapEnabled);
      ImGui::Checkbox("Normal", &renderer_settings.normalMapEnabled);
      ImGui::SameLine();
      ImGui::Checkbox("Emission", &renderer_settings.emissionMapEnabled);
      ImGui::TreePop();
    }

    bool vsync = m_window.IsVsync();
    if (ImGui::Checkbox("VSync", &vsync)) {
      m_window.SetVsync(vsync);
    }
    ImGui::Checkbox("MSAA", &renderer_settings.useMSAA);
    ImGui::Checkbox("Wireframe", &renderer_settings.wireframe);
    ImGui::Checkbox("Use Blinn", &renderer_settings.useBlinn);
    ImGui::Checkbox("Skybox", &renderer_settings.renderSkybox);
    ImGui::Checkbox("Render to ImGui Viewport", &m_renderToImGuiViewport);

    if (ImGui::Button("Recompile Shaders")) {
      m_renderer.RecompileShaders();
    }

    static char screenshot_buffer[50];
    ImGui::SameLine();
    if (ImGui::Button("Screenshot")) {
      m_screenshotFilename = screenshot_buffer;
      if (m_screenshotFilename.empty()) {
        m_renderer.Screenshot();
      } else {
        m_renderer.Screenshot(m_screenshotFilename);
      }
    }
    ImGui::InputTextWithHint("##screenshotFilename", "Screenshot Filename", screenshot_buffer,
                             sizeof(screenshot_buffer));
  }

  m_renderer.OnImGui();

  if (ImGui::CollapsingHeader("Camera")) {
    m_cameraController.OnImGui();
  }

  if (ImGui::CollapsingHeader("Lights")) {
    ImGui::Text("Enabled Lights");
    ImGui::Checkbox("Directional", &renderer_settings.renderDirectionalLights);
    ImGui::SameLine();
    ImGui::Checkbox("Point", &renderer_settings.renderPointLights);
    ImGui::SameLine();
    ImGui::Checkbox("Spot", &renderer_settings.renderSpotlights);
    m_sceneManager.GetActiveScene()->ImGuiLights();
  }

  ImGui::End();

  m_sceneManager.GetActiveScene()->OnImGui();

  if (demo_window) ImGui::ShowDemoWindow(&demo_window);

  // need to set focus on window if user is aiming camera in it
  static bool imgui_viewport_focused = false;
  static bool is_first_mouse = true;
  static ImGuiWindowFlags viewport_flags = ImGuiWindowFlags_NoNav;

  if (m_renderToImGuiViewport) {
    bool viewport_open = true;
    ImGui::Begin("Viewport", &viewport_open, viewport_flags);
    imgui_viewport_focused = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
    // ImGui flips UV coords, need to swap
    ImGui::Image((void *)static_cast<intptr_t>(m_renderer.GetFinalImageTexture().Id()),
                 GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

    // handle window resize
    static ImVec2 prev_dimensions{0, 0};
    ImVec2 curr_dimensions = ImGui::GetWindowSize();
    if (prev_dimensions.x != curr_dimensions.x || prev_dimensions.y != curr_dimensions.y) {
      // update cameras
      float aspect_ratio =
          static_cast<float>(curr_dimensions.x) / static_cast<float>(curr_dimensions.y);
      m_cameraController.SetAspectRatio(aspect_ratio);
    }
    prev_dimensions = curr_dimensions;

    // focus viewport window
    if (!m_cameraController.IsFocused() && ImGui::IsWindowHovered() &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      m_cameraController.Focus();
    }
    if (imgui_viewport_focused) {
      auto mouse_offset = Input::GetMousePosOffset();
      if (is_first_mouse) {
        mouse_offset = {0.0f, 0.0f};
        is_first_mouse = false;
      }
      if (mouse_offset != glm::vec2(0.0f)) {
        m_cameraController.ProcessMouseMovement(mouse_offset.x, mouse_offset.y);
      }
    } else {
      is_first_mouse = true;
    }
    ImGui::End();
  }
}

void Application::OnViewportResize(uint32_t width, uint32_t height) {
  m_renderer.ResizeViewport(width, height);
  float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
  m_cameraController.SetAspectRatio(aspect_ratio);
}

void Application::OnMousePosMove(double xpos, double ypos) {
  if (!m_renderToImGuiViewport && ImGui::GetIO().WantCaptureMouse) return;
  static bool is_first_mouse = true;
  static double last_x;
  static double last_y;
  if (is_first_mouse) {
    last_x = xpos;
    last_y = ypos;
    is_first_mouse = false;
  }

  double x_offset = xpos - last_x;
  double y_offset = ypos - last_y;

  last_x = xpos;
  last_y = ypos;

  if (!m_renderToImGuiViewport) {
    m_cameraController.ProcessMouseMovement(x_offset, y_offset);
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
  ShaderManager::AddShader("default", {{GET_SHADER_PATH("default.vert"), ShaderType::Vertex},
                                       {GET_SHADER_PATH("default.frag"), ShaderType::Fragment}});
  ShaderManager::AddShader("blinnPhong",
                           {{GET_SHADER_PATH("blinnPhong.vert"), ShaderType::Vertex},
                            {GET_SHADER_PATH("blinnPhong.frag"), ShaderType::Fragment}});
  ShaderManager::AddShader("skybox", {{GET_SHADER_PATH("skybox.vert"), ShaderType::Vertex},
                                      {GET_SHADER_PATH("skybox.frag"), ShaderType::Fragment}});
  ShaderManager::AddShader("singleColor",
                           {{GET_SHADER_PATH("singleColorStencil.vert"), ShaderType::Vertex},
                            {GET_SHADER_PATH("singleColorStencil.frag"), ShaderType::Fragment}});
  ShaderManager::AddShader(
      "instancedDefault",
      {{GET_SHADER_PATH("instancing/instanced.vert.glsl"), ShaderType::Vertex},
       {GET_SHADER_PATH("instancing/instanced.frag.glsl"), ShaderType::Fragment}});
}
