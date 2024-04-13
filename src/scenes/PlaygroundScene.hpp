//
// Created by Tony Adriansen on 3/4/24.
//

#ifndef OPENGL_RENDERER_SRC_SCENES_PLAYGROUNDSCENE_HPP_
#define OPENGL_RENDERER_SRC_SCENES_PLAYGROUNDSCENE_HPP_

#include "src/group/Scene.hpp"

class PlaygroundScene : public Scene {
 public:
  PlaygroundScene();
  void Update(double dt) override;

 private:
  std::vector<Object*> m_cubes;
};

#endif  // OPENGL_RENDERER_SRC_SCENES_PLAYGROUNDSCENE_HPP_
