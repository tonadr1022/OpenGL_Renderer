//
// Created by Tony Adriansen on 2/27/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_

#define GL_SILENCE_DEPRECATION 1
#include <glm/glm.hpp>
#include <src/Common.hpp>

class Window {
 public:
  Window();
  ~Window();
  static void SetVsync(bool state);
  void SwapBuffers();
  bool ShouldClose();
  void SetShouldClose(bool shouldClose);
  [[nodiscard]] float GetAspectRatio() const;
  [[nodiscard]] glm::ivec2 GetWindowDimensions() const;
  [[nodiscard]] glm::ivec2 GetFrameBufferDimensions() const;
  void Close();
  [[nodiscard]] inline GLFWwindow* GetContext() const { return m_window; }

 private:
  uint32_t m_framebufferWidth{}, m_framebufferHeight{};
  uint32_t m_windowWidth{}, m_windowHeight{};
  GLFWwindow* m_window = nullptr;
  const char* m_glsl_version = "#version 410";

  void Init_Glfw();
  void Init_ImGui();
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_WINDOW_HPP_
