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




Scene::Scene() : m_name("Default") {}

Scene::Scene(HashedString name) : m_name(name) {}
