#pragma once

#include <memory>

#include "src/group/Scene.hpp"

class InstancingScene : public Scene {
 public:
  InstancingScene();

 private:
  std::vector<std::unique_ptr<Group>> m_instanced_models;
};
