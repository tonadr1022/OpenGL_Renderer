//
// Created by Tony Adriansen on 2/27/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_

#include <cstdint>
#include "src/renderer/group/Scene.hpp"
#include "FrameCapturer.hpp"
#include "Window.hpp"

class Renderer {
 public:
  struct RenderSettings {
    bool wireframe{false};
    bool renderToImGuiViewport{false};
  };

  Renderer(Window& window, bool renderToImGuiViewport);
  void Init();
  void RenderScene(const Scene& scene);
  void SetActiveCamera(const Camera* camera);
  void SetWindowSize(uint32_t width, uint32_t height);

  struct PerFrameStats {
    uint32_t drawCalls{0};
    uint32_t vertices{0};
    uint32_t indices{0};
  };



  inline RenderSettings& GetSettings() {return m_settings; }
  inline const PerFrameStats& GetStats() {return stats; }
  inline const FrameCapturer& GetFrameCapturer() {return m_frameCapturer; }
 private:
  struct RenderState {
    const Material* boundMaterial = nullptr;
    Shader* boundShader = nullptr;
    const Camera* activeCamera = nullptr;
  };

  Window& m_window;
  RenderState state;
  FrameCapturer m_frameCapturer;
  PerFrameStats stats;
  RenderSettings m_settings;

  void UpdateRenderState(const Object& object);
  void ResetStats();
  void StartFrame(const Scene& scene);
  void EndFrame();
  void RenderGroup(const Group& group);

};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
