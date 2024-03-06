//
// Created by Tony Adriansen on 2/27/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_

#define GL_SILENCE_DEPRECATION 1
#include "src/Common.hpp"
#include <glm/glm.hpp>

class Window {
 public:
  void Init();
  ~Window();
  static void SetVsync(bool state);
  void StartFrame() const;
  void EndFrame();
  bool ShouldClose();
  void Close();
  inline GLFWwindow* GetContext() { return m_window; }
  [[nodiscard]] static inline double GetTime() { return glfwGetTime(); }
  float AspectRatio();
  void DockSpace(bool imguiFullScreen);
 private:
  GLFWwindow* m_window = nullptr;
  const char* m_glsl_version = "#version 410";

  void Init_Glfw();
  void Init_ImGui();

};

#endif //OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_
