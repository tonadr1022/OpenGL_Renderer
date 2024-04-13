//
// Created by Tony Adriansen on 3/6/24.
//

#ifndef OPENGL_RENDERER_SRC_IMGUI_IMGUIMENU_HPP_
#define OPENGL_RENDERER_SRC_IMGUI_IMGUIMENU_HPP_

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

// FROM:
// https://github.com/MorcilloSanz/RendererGL/blob/2af0f4d71f6a6148d30f292c6c7112177cb8c354/test/testImGuiGLFW/src/ImguiStyles.h

struct ImVec3 {
  float x, y, z;
  explicit ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
    x = _x;
    y = _y;
    z = _z;
  }
};

class ImGuiMenu {
 public:
  static void StartFrame(bool imguiFullscreen);
  static void EndFrame();
  static void Style();
};

#endif  // OPENGL_RENDERER_SRC_IMGUI_IMGUIMENU_HPP_
