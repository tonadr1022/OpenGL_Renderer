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

 private:
  std::vector<Group*> m_rotate_objs;
  std::vector<Group*> m_spots;
};

#endif  // OPENGL_RENDERER_SRC_SCENES_LIGHTINGONESCENE_HPP_
