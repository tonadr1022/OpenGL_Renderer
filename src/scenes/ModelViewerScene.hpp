//
// Created by Tony Adriansen on 3/16/24.
//

#ifndef OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_
#define OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_

#include "src/group/Scene.hpp"

class ModelViewerScene : public Scene {
 public:
  ModelViewerScene();
  void Update(double dt) override;
  void OnImGui() override;

 private:
  std::unordered_map<std::string, Group*> m_modelSelectMap;
  Group* m_visibleModel;
  std::string m_activeModelName;
  std::vector<std::string> m_skyboxNames;

  template <typename T>
  void ImGuiTransformComponent(T* object, const std::string& iStr);
};

#endif  // OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_
