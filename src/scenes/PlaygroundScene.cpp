//
// Created by Tony Adriansen on 3/4/24.
//

#include "PlaygroundScene.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "src/renderer/resource/MeshManager.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/renderer/camera/FPSCamera.hpp"

void PlaygroundScene::Update(double dt) {
  Scene::Update(dt);
  for (int i =0; i < 200; i++) {
    m_cubes[i]->transform.Rotate(0.5, {0.5f, 1.0f, 0.0f});
  }
}

PlaygroundScene::PlaygroundScene() {
  m_camera = std::make_unique<FPSCamera>();
  auto timeSetterFunction = [](HashedString id, Shader& shader) {
    shader.SetFloat(id, static_cast<float>(glfwGetTime()));
  };
  PerMaterialUniformData timeUniform{
      .id = "u_Time",
      .SetterFunction = timeSetterFunction
  };

  std::vector<Texture*> textures;
  std::vector<PerMaterialUniformData> pmd;
  pmd.push_back(timeUniform);
  textures.push_back(TextureManager::GetTexture("cow"));
  MaterialManager::AddMaterial("cowMat", textures, pmd, "default");
  auto g = std::make_unique<Group>();

  for (int i =0; i < 200; i++) {
    auto cube = std::make_unique<Object>(MeshManager::GetMesh("cube"), MaterialManager::GetMaterial("cowMat"));
    m_cubes.push_back(cube.get());
    cube.get()->transform.Translate(glm::vec3(i, 0, i));
    g->AddObject(std::move(cube));
  }

  AddGroup(std::move(g));
}
