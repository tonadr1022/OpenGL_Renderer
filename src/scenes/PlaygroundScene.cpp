//
// Created by Tony Adriansen on 3/4/24.
//

#include "PlaygroundScene.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "src/renderer/resource/MeshManager.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/core/Logger.hpp"
#include "src/renderer/shapes/Cube.hpp"
#include "src/Common.hpp"

void PlaygroundScene::Update(double dt) {
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

PlaygroundScene::PlaygroundScene() : Scene("Playground") {
  MeshManager::AddMesh("cube", Cube::Vertices, Cube::Indices);
  MeshManager::AddMesh("cube1024", Cube::Create(1024, 1024));
  TextureManager::AddTexture("cow", GET_TEXTURE_PATH("cow.png"));
  Texture* floorTex = TextureManager::AddTexture("floor", GET_TEXTURE_PATH("wood_floor.jpg"));
  Texture* containerTex = TextureManager::AddTexture("container", GET_TEXTURE_PATH("container.jpg"));
  floorTex->SetWrapMode(GL_REPEAT);
  containerTex->SetWrapMode(GL_REPEAT);

//  auto timeSetterFunction = [](HashedString id, Shader& shader) {
//    shader.SetFloat(id, static_cast<float>(glfwGetTime()));
//  };
//  PerMaterialUniformData timeUniform{
//      .id = "u_Time",
//      .SetterFunction = timeSetterFunction
//  };

  std::vector<Texture*> textures;
  std::vector<PerMaterialUniformData> pmd;
//  pmd.push_back(timeUniform);
  textures.push_back(TextureManager::GetTexture("cow"));
  MaterialManager::AddMaterial("cowMat", textures, pmd, "default");
  textures.clear();

//  pmd.push_back(timeUniform);
  textures.push_back(TextureManager::GetTexture("container"));
  MaterialManager::AddMaterial("floor", textures, pmd, "default");

  auto g = std::make_unique<Group>();

  auto plane = std::make_unique<Object>(MeshManager::GetMesh("cube"), MaterialManager::GetMaterial("floor"));
//  plane->transform.Scale({1000, 1, 1000});
//  plane->transform.UpdateModelMatrix();
  g->AddObject(std::move(plane));
//  g->transform.Scale({100, 1, 100});
  AddGroup(std::move(g));
}

