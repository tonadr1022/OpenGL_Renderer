//
// Created by Tony Adriansen on 3/6/24.
//

#include "SceneManager.hpp"
#include "src/core/Logger.hpp"

SceneManager::SceneManager() = default;

void SceneManager::AddScene(std::unique_ptr<Scene> scene) {
  auto res = m_sceneMap.emplace(scene->GetName(), std::move(scene));
  m_sceneNames.push_back(res.first->first);

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

const std::vector<HashedString>& SceneManager::GetSceneNames() {
  return m_sceneNames;
}
