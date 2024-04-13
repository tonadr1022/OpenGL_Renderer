//
// Created by Tony Adriansen on 3/6/24.
//

#include "SceneManager.hpp"
#include "src/utils/Logger.hpp"
#include <imgui/imgui.h>

SceneManager::SceneManager() = default;

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
  if (ImGui::BeginCombo("##SceneSelectCombo", ("Scene: " + std::string(m_activeSceneName)).c_str())) {
    for (auto& [name, scene] : m_sceneMap) {
      const bool isSelected = scene.get() == m_activeScene;
      if (ImGui::Selectable(name.data(), isSelected)) {
        m_activeScene = scene.get();
        if (isSelected) ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
}