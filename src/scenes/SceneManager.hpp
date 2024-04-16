//
// Created by Tony Adriansen on 3/6/24.
//

#ifndef OPENGL_RENDERER_SRC_SCENES_SCENEMANAGER_HPP_
#define OPENGL_RENDERER_SRC_SCENES_SCENEMANAGER_HPP_

#include <memory>

#include "src/camera/CameraController.hpp"
#include "src/group/Scene.hpp"
#include "src/renderer/Renderer.hpp"

class SceneManager {
 public:
  explicit SceneManager(Renderer& renderer, CameraController& camera_controller);
  void SetActiveScene(HashedString name);

  void AddScene(HashedString name, std::unique_ptr<Scene> scene);

  Scene* GetActiveScene();
  void ImGuiSceneSelect();

 private:
  Scene* m_activeScene = nullptr;
  HashedString m_activeSceneName;
  void OnSceneChange();
  std::unordered_map<HashedString, std::unique_ptr<Scene>> m_sceneMap;
  Renderer& m_renderer;
  CameraController& m_cameraController;
};

#endif  // OPENGL_RENDERER_SRC_SCENES_SCENEMANAGER_HPP_
