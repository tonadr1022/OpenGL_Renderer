//
// Created by Tony Adriansen on 3/8/24.
//

#include "LightingOneScene.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "src/renderer/resource/MeshManager.hpp"
#include "src/core/Logger.hpp"
#include "imgui/imgui.h"

LightingOneScene::LightingOneScene() : Scene("Lighting One", {1,2, 4}) {
  auto plane = std::make_unique<Object>(MeshManager::GetMesh("cube1024"), MaterialManager::GetMaterial("woodContainer"));
  plane->transform.Scale({1000, 1, 1000});
  plane->transform.UpdateModelMatrix();
  auto g = std::make_unique<Group>();
  g->AddObject(std::move(plane));

  for (int z = -20; z <= 20; z += 5) {
    for (int x = -20; x <= 20; x += 5) {
      auto cube = std::make_unique<Object>(MeshManager::GetMesh("cube"), MaterialManager::GetMaterial("woodContainer"));
      cube->transform.Translate({x, 3, z});
      m_cubes.push_back(cube.get());
      g->AddObject(std::move(cube));
    }
  }

  AddGroup(std::move(g));

  glm::vec3 color = {1, 0, 1};
  glm::vec3 pos = {1, 1, 1};
  m_lights.push_back(std::make_unique<Light>(pos, color, Light::Type::Point));
  color = {0, 1, 0};
  pos = {1, 1, 10};
  m_lights.push_back(std::make_unique<Light>(pos, color, Light::Type::Point));
  color = {0, 0, 1};
  pos = {4, 1, 0};
  m_lights.push_back(std::make_unique<Light>(pos, color, Light::Type::Point));
}

void LightingOneScene::Update(double dt) {
  for (auto& cube : m_cubes) {
    cube->transform.Rotate((float) dt * 10, {0.5f, 1.0f, 0.0f});
  }
  for (auto& cube : m_cubes) {
    cube->transform.UpdateModelMatrix();
  }

}

void LightingOneScene::OnImGui() {
  ImGui::Begin("Scene");
  int i = 0;
  for (auto& light : m_lights) {
    ImGui::Text("%s", Light::TypeToString(light->type));
    std::string label = "Color##" + std::to_string(i++);
    ImGui::ColorEdit3(label.c_str(), &light->color.x);
  }
  ImGui::End();
}