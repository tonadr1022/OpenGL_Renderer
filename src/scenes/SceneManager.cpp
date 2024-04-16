//
// Created by Tony Adriansen on 3/6/24.
//

#include "SceneManager.hpp"

#include <imgui/imgui.h>

#include "src/camera/CameraController.hpp"
#include "src/utils/Logger.hpp"

SceneManager::SceneManager(Renderer& renderer, CameraController& camera_controller)
    : m_renderer(renderer), m_cameraController(camera_controller) {}

void SceneManager::AddScene(HashedString name, std::unique_ptr<Scene> scene) {
  m_sceneMap.emplace(name, std::move(scene));
}

void SceneManager::SetActiveScene(HashedString name) {
  auto it = m_sceneMap.find(name);
  if (it == m_sceneMap.end()) LOG_ERROR("Scene not found: %s", name.data());
  m_activeScene = it->second.get();
  m_activeSceneName = name;
}

Scene* SceneManager::GetActiveScene() {
  if (m_activeScene == nullptr) LOG_ERROR("No active scene");
  return m_activeScene;
}

void SceneManager::ImGuiSceneSelect() {
  if (!m_activeScene || !m_activeSceneName) return;
  if (ImGui::BeginCombo("##SceneSelectCombo",
                        ("Scene: " + std::string(m_activeSceneName)).c_str())) {
    for (auto& [name, scene] : m_sceneMap) {
      const bool is_selected = scene.get() == m_activeScene;
      if (ImGui::Selectable(name.data(), is_selected)) {
        m_activeScene = scene.get();
        OnSceneChange();
        if (is_selected) ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
}

void SceneManager::OnSceneChange() {
  m_cameraController.SetMode(m_activeScene->defaultCameraMode);
  m_cameraController.GetActiveCamera()->SetPosition(m_activeScene->defaultCameraPosition);
  m_renderer.SetDirectionalLight(m_activeScene->GetDirectionalLight());
  m_renderer.SetPointLights(m_activeScene->GetPointLights());
  m_renderer.SetSpotLights(m_activeScene->GetSpotLights());
}
