//
// Created by Tony Adriansen on 3/6/24.
//

#ifndef OPENGL_RENDERER_SRC_SCENES_SCENEMANAGER_HPP_
#define OPENGL_RENDERER_SRC_SCENES_SCENEMANAGER_HPP_

#include "src/renderer/camera/FPSCamera.hpp"
#include "src/renderer/group/Scene.hpp"
#include <memory>

class SceneManager {
 public:
  SceneManager();
  void SetActiveScene(HashedString name);

  void AddScene(HashedString name, std::unique_ptr<Scene> scene);

  Scene* GetActiveScene();
  void ImGuiSceneSelect();

 private:
  Scene* m_activeScene = nullptr;
  HashedString m_activeSceneName;
  std::unordered_map<HashedString, std::unique_ptr<Scene>> m_sceneMap;
};

#endif //OPENGL_RENDERER_SRC_SCENES_SCENEMANAGER_HPP_
