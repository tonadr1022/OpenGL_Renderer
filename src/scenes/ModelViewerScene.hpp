//
// Created by Tony Adriansen on 3/16/24.
//

#ifndef OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_
#define OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_

#include "src/renderer/group/Scene.hpp"
#include "src/renderer/group/Model.hpp"

class ModelViewerScene : public Scene {
 public:
  ModelViewerScene();
  void Update(double dt) override;
  void OnImGui() override;
 private:
  std::unordered_map<std::string, Model*> m_modelSelectMap;
  Model* m_visibleModel;
  std::string m_activeModelName;
  template<typename T>
  void ImGuiTransformComponent(T* object, const std::string& iStr);
};

#endif //OPENGL_RENDERER_SRC_SCENES_MODELVIEWERSCENE_HPP_
