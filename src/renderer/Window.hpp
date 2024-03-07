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
  Window();
  ~Window();
  static void SetVsync(bool state);
  void SwapBuffers();
  bool ShouldClose();
  float GetAspectRatio() const;
  uint32_t GetWidth() const;
  uint32_t GetHeight() const;
  void Close();
  inline GLFWwindow* GetContext() { return m_window; }
 private:
  uint32_t m_width{}, m_height{};
  GLFWwindow* m_window = nullptr;
  const char* m_glsl_version = "#version 410";

  void Init_Glfw();
  void Init_ImGui();

};

#endif //OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_
