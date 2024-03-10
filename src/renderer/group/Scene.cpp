//
// Created by Tony Adriansen on 3/2/24.
//

#include "Scene.hpp"

void Scene::AddGroup(std::unique_ptr<Group> group) {
  m_groups.push_back(std::move(group));
}

void Scene::RemoveGroup(const Group* group) {
  int i = 0;
  for (auto& s : m_groups) {
    if (s.get() == group) {
      m_groups.erase(m_groups.begin() + i);
      break;
    }
    i++;
  }
}

void Scene::Update(double dt) {}

Scene::Scene() : defaultCameraPosition({0, 0, 0}), m_name("Default") {}

Scene::Scene(HashedString name) : defaultCameraPosition({0, 0, 0}), m_name(name) {}

Scene::Scene(HashedString name, const glm::vec3& defaultCameraPos)
    : defaultCameraPosition(defaultCameraPos), m_name(name) {}

void Scene::OnImGui() {}
