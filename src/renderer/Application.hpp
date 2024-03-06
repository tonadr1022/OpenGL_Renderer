//
// Created by Tony Adriansen on 2/28/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_

#include "Window.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include <memory>

class Application {
 public:
  static Application &Instance() { return *instancePtr; }
  Application();
  void Run();

 private:
  void ImGuiOptions();
  static Application *instancePtr;
  void OnResizeEvent(int width, int height);

  static void SetupResources();
  std::unique_ptr<Window> m_window;
  bool m_renderToImGuiViewport;
  Scene* m_activeScene{};
  std::vector<std::unique_ptr<Scene>> m_scenes;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
