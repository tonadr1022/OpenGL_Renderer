//
// Created by Tony Adriansen on 2/28/2024.
//

#include "Application.hpp"
#include "src/renderer/shapes/Cube.hpp"
#include "src/core/HashedString.hpp"
#include "src/renderer/resource/ShaderManager.hpp"
#include "src/renderer/resource/MeshManager.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/scenes/PlaygroundScene.hpp"
#include "imgui/imgui.h"

#define SHADER_PATH(filename) ("resources/shaders/" filename)
#define TEXTURE_PATH(filename) ("resources/textures/" filename)

Application* Application::instancePtr = nullptr;

Application::Application() {
  instancePtr = this;
  m_window = std::make_unique<Window>();
  m_window->Init();
  m_renderToImGuiViewport = true;
  Renderer::SetImGuiFullScreen(m_renderToImGuiViewport);
}

void Application::SetupResources() {
  ShaderManager::AddShader("default", {{SHADER_PATH("default.vert"), ShaderType::VERTEX},
                                       {SHADER_PATH("default.frag"), ShaderType::FRAGMENT}});
  MeshManager::AddMesh("cube", Cube::Vertices, Cube::Indices);
  TextureManager::AddTexture("cow", TEXTURE_PATH("cow.png"));
}

void Application::Run() {
  SetupResources();
  Renderer::Init();

  m_scenes.push_back(std::make_unique<PlaygroundScene>());
  m_activeScene = m_scenes[0].get();

  float currTime, deltaTime, lastTime = glfwGetTime();
  while (!m_window->ShouldClose()) {
    currTime = glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;

    // input
    Input::Update();

    // update
    m_activeScene->Update(deltaTime);


    // render
    m_window->StartFrame();
    m_window->DockSpace(m_renderToImGuiViewport);

    Renderer::RenderScene(*m_activeScene);
    ImGuiOptions();
    m_window->EndFrame();
  }
}

void Application::ImGuiOptions() {
  ImGui::Begin("Settings");
  if (ImGui::Checkbox("Render to ImGui Viewport", &m_renderToImGuiViewport)) {
    Renderer::SetImGuiFullScreen(m_renderToImGuiViewport);
  }
  ImGui::End();
}