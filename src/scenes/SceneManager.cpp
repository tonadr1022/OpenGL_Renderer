//
// Created by Tony Adriansen on 3/6/24.
//

#include "SceneManager.hpp"
#include "src/core/Logger.hpp"
#include <imgui/imgui.h>

SceneManager::SceneManager() = default;

void SceneManager::AddScene(std::unique_ptr<Scene> scene) {
   m_sceneMap.emplace(scene->GetName(), std::move(scene));
}

void SceneManager::SetActiveScene(HashedString name) {
  auto it = m_sceneMap.find(name);
  if (it == m_sceneMap.end()) LOG_ERROR("Scene not found: %s", name.data());
  m_activeScene = it->second.get();
}

Scene* SceneManager::GetActiveScene() {
  if (m_activeScene == nullptr) LOG_ERROR("No active scene");
  return m_activeScene;
}

void SceneManager::ImGuiSceneSelect() {
  if (!m_activeScene) return;
  if (ImGui::BeginCombo("##SceneSelectCombo", ("Scene: " + std::string(m_activeScene->GetName())).c_str())) {
    for (auto& [first, scene] : m_sceneMap) {
      const bool isSelected = scene.get() == m_activeScene;
      if (ImGui::Selectable(scene->GetName(), isSelected)) {
        m_activeScene = scene.get();
        if (isSelected) ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
}