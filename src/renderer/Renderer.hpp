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
  enum class Mode {
    None, BlinnPhong
  };
  struct RenderSettings {
    bool wireframe{false};
    bool renderToImGuiViewport{false};
  };

  Renderer(Window& window, bool renderToImGuiViewport);
  void Init();
  void RenderScene(const Scene& scene, Camera* camera);
  void SetFrameBufferSize(uint32_t width, uint32_t height);
  void SetLights(const std::vector<std::unique_ptr<Light>>& lights);

  struct PerFrameStats {
    uint32_t drawCalls{0};
    uint32_t vertices{0};
    uint32_t indices{0};
  };

  inline RenderSettings& GetSettings() { return m_settings; }
  inline const PerFrameStats& GetStats() { return stats; }
  inline const FrameCapturer& GetFrameCapturer() { return m_frameCapturer; }

  Mode mode = Mode::BlinnPhong;
 private:
  struct RenderState {
    const Material* boundMaterial = nullptr;
    Shader* boundShader = nullptr;
  };

  const std::vector<std::unique_ptr<Light>>* m_lights = nullptr;

  Window& m_window;
  Camera* m_camera = nullptr;
  RenderState state;
  FrameCapturer m_frameCapturer;
  PerFrameStats stats;
  RenderSettings m_settings;

  void UpdateRenderState(const Object& object);
  void ResetStats();
  void StartFrame(const Scene& scene);
  void EndFrame();
  void RenderGroup(const Group& group);
  void SetLightingUniforms();

};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
