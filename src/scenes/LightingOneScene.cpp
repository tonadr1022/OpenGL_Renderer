//
// Created by Tony Adriansen on 3/8/24.
//

#include "LightingOneScene.hpp"

#include <iostream>
#include <memory>

#include "src/camera/CameraController.hpp"
#include "src/renderer/Light.hpp"
#include "src/resource/MaterialManager.hpp"
#include "src/resource/MeshManager.hpp"
#include "src/resource/ModelManager.hpp"
#include "src/utils/HashedString.hpp"

LightingOneScene::LightingOneScene() : Scene({10, 10, 10}, "", CameraController::Mode::Orbit) {
  Material* wood_container_mat = MaterialManager::GetMaterial("woodContainer");
  Material* wood_container_emission_mat = MaterialManager::GetMaterial("woodContainerEmission");

  Mesh* cube_mesh = MeshManager::GetMesh("cube");

  auto plane = std::make_unique<Object>(MeshManager::GetMesh("cube1024"), wood_container_mat);
  plane->transform.Scale({1000, 1, 1000});
  auto plane_group = std::make_unique<Group>();
  plane_group->AddObject(std::move(plane));

  auto spot = ModelManager::CopyLoadedModel("spot");
  spot->SetMaterial(MaterialManager::GetMaterial("spotTextured"));
  auto dragon = ModelManager::CopyLoadedModel("dragon");
  dragon->transform.SetScale({0.1, 0.1, 0.1});
  MaterialManager::AddMaterial("dragonLightingScene", glm::vec3{1.0, 0.0, 0.5},
                               Material::Type::BlinnPhong);
  dragon->SetMaterial(MaterialManager::GetMaterial("dragonLightingScene"));

  auto cube_obj = std::make_unique<Object>(cube_mesh, wood_container_mat);
  auto cube = std::make_unique<Group>();
  cube->AddObject(std::move(cube_obj));
  auto cube_emission = std::make_unique<Group>(*cube);
  cube_emission->SetMaterial(wood_container_emission_mat);

  std::vector<Group*> groups = {spot.get(), dragon.get(), cube_emission.get(), cube.get()};

  for (int z = -4; z <= 4; z++) {
    for (int x = -4; x <= 4; x++) {
      auto g = std::make_unique<Group>(
          *groups[((z + x) % groups.size() + groups.size()) % groups.size()]);
      g->transform.Translate({z * 4, 3, x * 4});
      m_rotate_objs.push_back(g.get());
      AddGroup(std::move(g));
    }
  }

  // for (int z = -200; z <= 20; z += 5) {
  //   for (int x = -20; x <= 20; x += 5) {
  //     if (x % 2 == 0) {
  //       group->transform.Translate({x, 3, z});
  //       m_rotate_objs.push_back(group.get());
  //       AddGroup(std::move(group));
  //     } else {
  //       auto spot = ModelManager::CopyLoadedModel("spot");
  //       auto name = HashedString(
  //           static_cast<std::string>(std::to_string(x) + std::to_string(z) + "light").c_str());
  //
  //       MaterialManager::AddMaterial(name, glm::vec3(rand(), rand(), rand()),
  //                                    Material::Type::BlinnPhong);
  //       auto* mat = MaterialManager::GetMaterial(name);
  //       for (const auto& obj : spot->GetObjects()) {
  //         obj->SetMaterial(mat);
  //       }
  //       spot->transform.Translate({x, 3, z});
  //       m_rotate_objs.push_back(spot.get());
  //       AddGroup(std::move(spot));
  //     }
  //   }
  // }

  // for (int z = -200; z <= 20; z += 5) {
  //   for (int x = -20; x <= 20; x += 5) {
  //     auto cube = std::make_unique<Object>(cube_mesh, wood_container_mat);
  //     cube->transform.Translate({x, 3, z});
  //     m_cubes.push_back(cube.get());
  //     cube_group->AddObject(std::move(cube));
  //   }
  // }

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
  auto spotlight = std::make_unique<SpotLight>(glm::vec3{0, 8, 0}, dir);
  spotlight->color = {0.0, 0.0, 1.0};
  spotlight->penumbra = 10;
  spotlight->angle = 45;
  m_spotLights.emplace_back(std::move(spotlight));
}

void LightingOneScene::Update(double dt) {
  for (auto& obj : m_rotate_objs) {
    obj->transform.Rotate(static_cast<float>(dt) * 10, {0.5f, 1.0f, 0.0f});
  }
}
