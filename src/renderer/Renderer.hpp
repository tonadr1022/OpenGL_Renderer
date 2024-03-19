//
// Created by Tony Adriansen on 2/27/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_

#include <cstdint>
#include "src/renderer/group/Scene.hpp"
#include "src/renderer/Skybox.hpp"
#include "FrameCapturer.hpp"
#include "Window.hpp"
#include "Quad.hpp"

class Renderer {
 public:
  enum class Mode {
    None, BlinnPhong
  };
  enum class DebugMode {
    None, Normals, Diffuse, DepthBuffer
  };

  struct RenderSettings {
    bool wireframe{false};
    bool renderToImGuiViewport{false};
    bool renderDirectionalLights{true};
    bool renderSpotlights{true};
    bool renderPointLights{true};
    bool renderSkybox{true};
    bool useBlinn{true};

    bool diffuseMapEnabled{true};
    bool specularMapEnabled{true};
    bool normalMapEnabled{true};
    bool emissionMapEnabled{true};
  };

  Renderer(Window& window, bool renderToImGuiViewport);
  void Init();
  void RenderScene(const Scene& scene, Camera* camera);
  void SetFrameBufferSize(uint32_t width, uint32_t height);
  void SetDirectionalLight(const DirectionalLight* directionalLight);
  void SetSpotLights(const std::vector<std::unique_ptr<SpotLight>>* spotLights);
  void SetPointLights(const std::vector<std::unique_ptr<PointLight>>* pointLights);
  void Reset();

  void RecompileShaders();

  struct PerFrameStats {
    uint32_t drawCalls{0};
    uint32_t vertices{0};
    uint32_t indices{0};
  };

  inline RenderSettings& GetSettings() { return m_settings; }
  inline const PerFrameStats& GetStats() { return stats; }
  inline const FrameCapturer& GetFrameCapturer() { return m_frameCapturer; }

  Mode mode = Mode::BlinnPhong;
  DebugMode debugMode = DebugMode::None;


 private:
  struct RenderState {
    const Material* boundMaterial = nullptr;
    Shader* boundShader = nullptr;
    HashedString boundShaderName;
  };

  const DirectionalLight* m_directionalLight = nullptr;
  const std::vector<std::unique_ptr<PointLight>>* m_pointLights = nullptr;
  const std::vector<std::unique_ptr<SpotLight>>* m_spotLights = nullptr;

  Shader* m_screenShader = nullptr;
  Shader* m_skyboxShader = nullptr;

  Quad m_screenQuad;
  Skybox m_skybox;

  Window& m_window;
  Camera* m_camera = nullptr;
  RenderState state;
  FrameCapturer m_frameCapturer;
  PerFrameStats stats;
  RenderSettings m_settings;

  void UpdateRenderState(const Object& object);
  void ResetStats();
  void StartFrame(const Scene& scene);
  void RenderGroup(const Group& group);
  void ApplyPostProcessingEffects();
  void SetLightingUniforms();
  void SetBlinnPhongUniforms();
  void RenderSkybox(Camera* camera);

  void AssignShaders();

};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
