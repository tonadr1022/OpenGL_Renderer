//
// Created by Tony Adriansen on 3/16/24.
//

#include "ModelViewerScene.hpp"
#include "src/renderer/group/Model.hpp"

ModelViewerScene::ModelViewerScene() : Scene("Model Viewer", {2, 2, 2}) {
  auto backpack = std::make_unique<Model>("resources/models/backpack/backpack.obj");
  m_groups.push_back(std::move(backpack));

  glm::vec3 directionalDir = {0.2f, -0.5f, 0.5f};
  m_directionalLight = std::make_unique<DirectionalLight>(directionalDir);

  m_pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3(10, 4, 1)));

}
void ModelViewerScene::Update(double dt) {
  Scene::Update(dt);
}
void ModelViewerScene::OnImGui() {
  Scene::OnImGui();
}
