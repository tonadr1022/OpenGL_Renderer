//
// Created by Tony Adriansen on 2/27/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_

#include <cstdint>

#include "PostProcessor.hpp"
#include "Quad.hpp"
#include "src/Window.hpp"
#include "src/gl/FBOContainer.hpp"
#include "src/group/Scene.hpp"
#include "src/renderer/Skybox.hpp"

class Renderer {
 public:
  enum class Mode { None, BlinnPhong };
  enum class DebugMode { None, Normals, Diffuse, DepthBuffer };

  struct RenderSettings {
    bool wireframe{false};
    bool renderDirectionalLights{true};
    bool renderSpotlights{true};
    bool renderPointLights{true};
    bool renderSkybox{true};
    bool useBlinn{true};
    bool useMSAA{true};

    bool diffuseMapEnabled{true};
    bool specularMapEnabled{true};
    bool normalMapEnabled{true};
    bool emissionMapEnabled{true};
  };

  struct PostProcessingUniforms {
    float contrast = 0.0f;
    bool invert = false;
    bool flip = false;
  };

  PostProcessingUniforms ppUniforms;

  explicit Renderer(Window& window);
  void Init();
  void RenderScene(const Scene& scene, Camera* camera);
  void ResizeViewport(uint32_t width, uint32_t height);
  void SetDirectionalLight(const DirectionalLight* directionalLight);
  void SetSpotLights(const std::vector<std::unique_ptr<SpotLight>>* spotLights);
  void SetPointLights(const std::vector<std::unique_ptr<PointLight>>* pointLights);
  void OnImGui();

  void RecompileShaders();

  struct PerFrameStats {
    uint32_t drawCalls{0};
    uint32_t vertices{0};
    uint32_t indices{0};
    uint32_t numShaderBinds{0};
    uint32_t numMaterialSwitches{0};
  };

  inline RenderSettings& GetSettings() { return m_settings; }
  inline const PerFrameStats& GetStats() { return m_stats; }

  Mode mode = Mode::BlinnPhong;
  DebugMode debugMode = DebugMode::None;

  void Screenshot(std::string_view filename);
  void Screenshot();
  const Texture& GetFinalImageTexture();

 private:
  struct RenderState {
    const Material* boundMaterial = nullptr;
    Shader* boundShader = nullptr;
    HashedString boundShaderName;
  };

  const DirectionalLight* m_directionalLight = nullptr;
  const std::vector<std::unique_ptr<PointLight>>* m_pointLights = nullptr;
  const std::vector<std::unique_ptr<SpotLight>>* m_spotLights = nullptr;

  Shader* m_contrastShader = nullptr;
  Shader* m_invertShader = nullptr;
  Shader* m_skyboxShader = nullptr;
  Shader* m_stencilShader = nullptr;
  Shader* m_defaultInstancedShader = nullptr;

  Quad m_screenQuad;
  Skybox m_skybox;

  Window& m_window;
  Camera* m_camera = nullptr;
  Texture* m_skyboxTexture = nullptr;

  RenderState m_state;

  std::unique_ptr<FBOContainer> m_multiSampleFBOContainer = nullptr;
  std::unique_ptr<FBOContainer> m_singleSampleFBOContainer = nullptr;
  std::unique_ptr<FBOContainer> m_resolveSampleFBOContainer = nullptr;
  std::unique_ptr<FBOContainer> m_finalTextureFBO = nullptr;

  PerFrameStats m_stats;
  RenderSettings m_settings;
  PostProcessor m_postProcessor;

  uint32_t m_width{}, m_height{};

  void UpdateRenderState(const Object& object);
  void ResetStats();
  void IncStats(uint32_t numVertices, uint32_t numIndices);
  void StartFrame(const Scene& scene);
  void RenderGroup(const Group& group);
  void SetLightingUniforms();
  void SetBlinnPhongShaderUniforms() const;
  void RenderSkybox(Camera* camera);
  void AllocateFBOContainers(uint32_t width, uint32_t height);

  void AssignShaders();
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_GL_RENDERER_HPP_
