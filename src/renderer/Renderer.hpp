//
// Created by Tony Adriansen on 2/27/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_

#include <cstdint>
#include "src/renderer/group/Scene.hpp"

namespace Renderer {
void Init();
void RenderScene(const Scene& scene);
float GetAspectRatio();
void SetWindowSize(int width, int height);
void SetImGuiFullScreen(bool imguiFullScreen);

struct PerFrameStats {
  uint32_t drawCalls{0};
  uint32_t vertices{0};
  uint32_t indices{0};
};
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
