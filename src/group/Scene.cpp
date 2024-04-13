//
// Created by Tony Adriansen on 3/2/24.
//

#include "Scene.hpp"

#include "imgui/imgui.h"

void Scene::AddGroup(std::unique_ptr<Group> group) { m_groups.push_back(std::move(group)); }

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

Scene::Scene() : defaultCameraPosition({0, 0, 0}) {}

Scene::Scene(const glm::vec3& defaultCameraPos, CameraController::Mode defaultCamMode)
    : defaultCameraPosition(defaultCameraPos), defaultCameraMode(defaultCamMode) {}

void Scene::ImGuiLights() {
  if (m_directionalLight != nullptr) {
    ImGui::Text("Directional");
    ImGui::DragFloat3("Direction", &m_directionalLight->direction.x, 0.001);
    ImGui::ColorEdit3("Color", &m_directionalLight->color.x);
  }
  int i = 0;
  ImGui::Text("Point Lights");
  for (auto& light : m_pointLights) {
    std::string i_str = std::to_string(i);
    ImGui::DragFloat3(("Position##" + i_str).c_str(), &light->position.x, 0.1);
    ImGui::ColorEdit3(("Color##" + i_str).c_str(), &light->color.x);
    ImGui::DragFloat(("Intensity##" + i_str).c_str(), &light->radius, 0.1, 0, 100);
    i++;
  }

  ImGui::Text("Spotlights");
  i = 0;
  for (auto& light : m_spotLights) {
    std::string i_str = std::to_string(i);
    ImGui::DragFloat3(("Position##" + i_str).c_str(), &light->position.x, 0.1);
    ImGui::ColorEdit3(("Color##" + i_str).c_str(), &light->color.x);
    ImGui::DragFloat(("Intensity##" + i_str).c_str(), &light->radius, 1, 100);
    ImGui::DragFloat(("Angle##" + i_str).c_str(), &light->angle, 0.1, 1, 90);
    ImGui::DragFloat(("Penumbra##" + i_str).c_str(), &light->penumbra, 0.1, 0, light->angle);
    i++;
  }
}

const DirectionalLight* Scene::GetDirectionalLight() const {
  if (m_directionalLight == nullptr) return nullptr;
  return m_directionalLight.get();
}

void Scene::OnImGui() {}

void Scene::PreRender() {
  for (auto& group : m_groups) {
    group->UpdateTransforms();
  }
}
