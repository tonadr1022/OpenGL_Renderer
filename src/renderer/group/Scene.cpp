//
// Created by Tony Adriansen on 3/2/24.
//

#include "Scene.hpp"
#include "imgui/imgui.h"

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

void Scene::ImGuiLights() {
  if (m_directionalLight != nullptr) {
    ImGui::Text("Directional");
    ImGui::DragFloat3("Direction", &m_directionalLight->direction.x, 0.001);
    ImGui::ColorEdit3("Color", &m_directionalLight->color.x);
  }
  int i = 0;
  ImGui::Text("Point Lights");
  for (auto& light : m_pointLights) {
    std::string iStr = std::to_string(i);
    ImGui::DragFloat3(("Position##" + iStr).c_str(), &light->position.x, 0.1);
    ImGui::ColorEdit3(("Color##" + iStr).c_str(), &light->color.x);
//    ImGui::DragFloat("Intensity")
    i++;
  }

  ImGui::Text("Spotlights");
  i = 0;
  for (auto& light : m_spotLights) {
    std::string iStr = std::to_string(i);
    ImGui::DragFloat3(("Position##" + iStr).c_str(), &light->position.x, 0.1);
    ImGui::ColorEdit3(("Color##" + iStr).c_str(), &light->color.x);
    ImGui::DragFloat(("Angle##" + iStr).c_str(), &light->angle, 0.1, 1, 90);
    ImGui::DragFloat(("Penumbra##" + iStr).c_str(), &light->penumbra, 0.1, 0, light->angle);
  }
}

const DirectionalLight* Scene::GetDirectionalLight() const {
  if (m_directionalLight == nullptr) return nullptr;
  return m_directionalLight.get();
}

void Scene::OnImGui() {

}
