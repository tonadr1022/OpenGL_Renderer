//
// Created by Tony Adriansen on 2/28/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_

#include "Window.hpp"
#include "Input.hpp"
#include "src/renderer/gl/Renderer.hpp"

class Application {
 public:
  Application();
  void Run();

 private:
  std::unique_ptr<Window> m_window;
  std::unique_ptr<Renderer> m_renderer;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_APPLICATION_HPP_
