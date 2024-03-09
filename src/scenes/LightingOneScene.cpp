//
// Created by Tony Adriansen on 3/8/24.
//

#include "LightingOneScene.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "src/renderer/resource/MeshManager.hpp"
#include "src/core/Logger.hpp"

LightingOneScene::LightingOneScene() : Scene("Lighting One") {
  auto plane = std::make_unique<Object>(MeshManager::GetMesh("cube1024"), MaterialManager::GetMaterial("floor"));
  plane->transform.Scale({1000, 1, 1000});
  plane->transform.UpdateModelMatrix();

  auto g = std::make_unique<Group>();
  g->AddObject(std::move(plane));
  AddGroup(std::move(g));
}

void LightingOneScene::Update(double dt) {

}
