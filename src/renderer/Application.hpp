//
// Created by Tony Adriansen on 2/28/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_

#include "Window.hpp"
#include "Input.hpp"
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


 private:
  void OnImGui();
  static Application* instancePtr;
  static void SetupResources();
  Window m_window;
  CameraController m_cameraController;
  SceneManager m_sceneManager;
  Renderer m_renderer;


  bool m_renderToImGuiViewport;

};

#endif //OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
