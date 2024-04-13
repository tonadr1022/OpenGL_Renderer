//
// Created by Tony Adriansen on 3/8/24.
//

#ifndef OPENGL_RENDERER_SRC_SCENES_LIGHTINGONESCENE_HPP_
#define OPENGL_RENDERER_SRC_SCENES_LIGHTINGONESCENE_HPP_

#include <src/group/Scene.hpp>
class LightingOneScene : public Scene {
 public:
  LightingOneScene();
  void Update(double dt) override;
  void OnImGui() override;

 private:
  std::vector<Object*> m_cubes;
};

#endif  // OPENGL_RENDERER_SRC_SCENES_LIGHTINGONESCENE_HPP_
