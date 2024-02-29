//
// Created by Tony Adriansen on 2/27/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_

#include "Common.hpp"

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

 private:
  GLFWwindow* m_window;
  uint32_t m_windowWidth{1};
  uint32_t m_windowHeight{1};
  const char* m_glsl_version = "#version 130";
  glm::vec4 m_clearColor = glm::vec4(1.0f);

  void Init_Glfw();
  void Init_ImGui();

  static void glfw_error_cb(int error);
  static void framebuffer_size_cb(GLFWwindow* window, int width, int height);
};

#endif //OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_
