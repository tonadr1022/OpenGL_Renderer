//
// Created by Tony Adriansen on 3/16/24.
//

#ifndef OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_
#define OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_

#include "src/renderer/group/Scene.hpp"

class ModelViewerScene : public Scene {
 public:
  ModelViewerScene();
  void Update(double dt) override;
  void OnImGui() override;

};

#endif //OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_
