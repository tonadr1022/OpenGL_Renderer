//
// Created by Tony Adriansen on 3/6/24.
//

#ifndef OPENGL_RENDERER_SRC_IMGUI_IMGUIMENU_HPP_
#define OPENGL_RENDERER_SRC_IMGUI_IMGUIMENU_HPP_

#include <cstdint>

class ImGuiMenu {
 public:
  static void StartFrame(bool imguiFullscreen);
  static void EndFrame();
};

#endif //OPENGL_RENDERER_SRC_IMGUI_IMGUIMENU_HPP_
