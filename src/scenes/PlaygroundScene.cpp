//
// Created by Tony Adriansen on 3/4/24.
//

#include "PlaygroundScene.hpp"

#include "src/resource/MaterialManager.hpp"
#include "src/resource/MeshManager.hpp"

void PlaygroundScene::Update(double /*dt*/) {
  //  for (auto& cube : m_cubes) {
  //    cube->transform.Rotate((float) dt * 10, {0.5f, 1.0f, 0.0f});
  //  }
  for (auto& cube : m_cubes) {
    cube->transform.UpdateModelMatrix();
  }
  //  for (auto& group : m_groups) {
  //    group->transform.UpdateModelMatrix();
  //  }
}

PlaygroundScene::PlaygroundScene() {
  //  auto timeSetterFunction = [](HashedString id, Shader& shader) {
  //    shader.SetFloat(id, static_cast<float>(glfwGetTime()));
  //  };
  //  PerMaterialUniformData timeUniform{
  //      .id = "u_Time",
  //      .SetterFunction = timeSetterFunction
  //  };

  //  textures.push_back(TextureManager::GetTexture("woodContainer"));
  //  MaterialManager::AddMaterial("floor", textures, "blinnPhong");

  auto g = std::make_unique<Group>();
  auto plane = std::make_unique<Object>(MeshManager::GetMesh("cube"),
                                        MaterialManager::GetMaterial("woodContainer"));
  g->AddObject(std::move(plane));
  AddGroup(std::move(g));
}
