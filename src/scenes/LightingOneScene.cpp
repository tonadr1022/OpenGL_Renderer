//
// Created by Tony Adriansen on 3/8/24.
//

#include "LightingOneScene.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "src/renderer/resource/MeshManager.hpp"
#include "src/core/Logger.hpp"
#include "imgui/imgui.h"
#include "src/renderer/group/Model.hpp"

LightingOneScene::LightingOneScene() : Scene("Lighting One", {1, 2, 4}) {
  auto plane = std::make_unique<Object>(MeshManager::GetMesh("cube1024"), MaterialManager::GetMaterial("woodContainer"));
  plane->transform.Scale({1000, 1, 1000});
  plane->transform.UpdateModelMatrix();
  auto g = std::make_unique<Group>();
  g->AddObject(std::move(plane));

  for (int z = -200; z <= 20; z += 5) {
    for (int x = -20; x <= 20; x += 5) {
      auto cube = std::make_unique<Object>(MeshManager::GetMesh("cube"), MaterialManager::GetMaterial("woodContainer"));
      cube->transform.Translate({x, 3, z});
      m_cubes.push_back(cube.get());
      g->AddObject(std::move(cube));
    }
  }
  for (auto& cube : m_cubes) {
    cube->transform.UpdateModelMatrix();
  }

  AddGroup(std::move(g));

  glm::vec3 directionalDir = {0.2f, -0.5f, 0.5f};
  m_directionalLight = std::make_unique<DirectionalLight>(directionalDir);
  glm::vec3 iter = {1, 1, 0};

  for (iter.x = 0.0f; iter.x < 20.0f; iter.x += 10.0f) {
    for (iter.z = -10.0f; iter.z <= 0.0f; iter.z += 10.0f) {
      m_pointLights.emplace_back(std::make_unique<PointLight>(iter));
    }
  }

glm::vec3 dir = {0,-1,0};
m_spotLights.emplace_back(std::make_unique<SpotLight>(iter, dir));
}

void LightingOneScene::Update(double dt) {
//  for (auto& cube : m_cubes) {
//    cube->transform.Rotate((float) dt * 10, {0.5f, 1.0f, 0.0f});
//  }
//  for (auto& cube : m_cubes) {
//    cube->transform.UpdateModelMatrix();
//  }

}

void LightingOneScene::OnImGui() {
  Scene::OnImGui();
}