//
// Created by Tony Adriansen on 2/28/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_

#include "Window.hpp"
#include "src/core/Input.hpp"
#include "Renderer.hpp"
#include "src/scenes/SceneManager.hpp"
#include "src/renderer/camera/CameraController.hpp"

#include <memory>

class Application {
 public:
  enum class CameraMode {
    NONE, FPS, ORBIT
  };
  static Application& Instance() { return *instancePtr; }
  Application();
  void Run();

  void OnViewportResize(uint32_t width, uint32_t height);
  void OnMousePosMove(double xoffset, double yoffset);
  void OnMouseButtonEvent(int button, int action);
  void OnMouseScrollEvent(double yOffset);
  void OnKeyEvent(int key, int action, int mods);
  inline Renderer& GetRenderer() { return m_renderer;}

 private:
  void OnSceneChange();
  void OnImGui();
  static Application* instancePtr;
  static void SetupResources();
  Window m_window;
  CameraController m_cameraController;
  SceneManager m_sceneManager;
  Renderer m_renderer;

  struct Settings {
    bool showImGui = true;
  };

  Settings m_settings;


  bool m_renderToImGuiViewport;

};

#endif //OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
