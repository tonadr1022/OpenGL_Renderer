#pragma once

#include <memory>

#include "src/group/Scene.hpp"

class InstancingScene : public Scene {
 public:
  InstancingScene();
  void Update(double dt) override;

 private:
  std::vector<std::unique_ptr<Group>> m_instanced_models;
  std::vector<glm::mat4> m_model_matrices;
};
