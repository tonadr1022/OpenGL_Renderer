//
// Created by Tony Adriansen on 3/8/24.
//

#include "LightingOneScene.hpp"

#include "src/resource/MaterialManager.hpp"
#include "src/resource/MeshManager.hpp"

LightingOneScene::LightingOneScene() : Scene({1, 2, 4}) {
  Material* wood_container_mat = MaterialManager::GetMaterial("woodContainer");
  Mesh* cube_mesh = MeshManager::GetMesh("cube");

  auto plane = std::make_unique<Object>(MeshManager::GetMesh("cube1024"), wood_container_mat);
  plane->transform.Scale({1000, 1, 1000});
  plane->transform.UpdateModelMatrix();
  auto plane_group = std::make_unique<Group>();
  plane_group->AddObject(std::move(plane));

  auto cube_group = std::make_unique<Group>();
  for (int z = -200; z <= 20; z += 5) {
    for (int x = -20; x <= 20; x += 5) {
      auto cube = std::make_unique<Object>(cube_mesh, wood_container_mat);
      cube->transform.Translate({x, 3, z});
      m_cubes.push_back(cube.get());
      cube_group->AddObject(std::move(cube));
    }
  }
  for (auto& cube : m_cubes) {
    cube->transform.UpdateModelMatrix();
  }
  AddGroup(std::move(cube_group));
  AddGroup(std::move(plane_group));

  //  auto backpack = ModelManager::CopyLoadedModel("backpack");
  // copy the contents of backpack 10 times
  //  for (int i =0; i < 10; i++) {
  //    auto bpI = std::make_unique<Group>(*backpack);
  //    bpI->transform.Translate(glm::vec3(i*10,0,i*10));
  //    m_groups.emplace_back(std::move(bpI));
  //  }

  glm::vec3 directional_dir = {0.2f, -0.5f, 0.5f};
  m_directionalLight = std::make_unique<DirectionalLight>(directional_dir);
  glm::vec3 iter = {1, 1, 0};

  for (iter.x = 0.0f; iter.x < 20.0f; iter.x += 10.0f) {
    for (iter.z = -10.0f; iter.z <= 0.0f; iter.z += 10.0f) {
      m_pointLights.emplace_back(std::make_unique<PointLight>(iter));
    }
  }

  glm::vec3 dir = {0, -1, 0};
  m_spotLights.emplace_back(std::make_unique<SpotLight>(iter, dir));
}

void LightingOneScene::Update(double dt) {
  // for (auto& cube : m_cubes) {
  //   cube->transform.Rotate(static_cast<float>(dt) * 10, {0.5f, 1.0f, 0.0f});
  // }
  //  for (auto& cube : m_cubes) {
  //    cube->transform.UpdateModelMatrix();
  //  }
}

void LightingOneScene::OnImGui() { Scene::OnImGui(); }
