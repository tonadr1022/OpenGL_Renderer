//
// Created by Tony Adriansen on 2/28/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_

#include "Window.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "src/renderer/camera/FPSCamera.hpp"
#include "src/scenes/SceneManager.hpp"

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
  void OnMousePosMove(float xoffset, float yoffset);
  void OnMouseButtonEvent(int button, int action);

 private:
  void DisplayImGui();
  static Application* instancePtr;
  static void SetupResources();
  Window m_window;
  SceneManager m_sceneManager;
  Renderer m_renderer;

  CameraMode m_cameraMode;
  std::unique_ptr<FPSCamera> m_fpsCamera;
  bool m_renderToImGuiViewport;
  bool m_focused;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
