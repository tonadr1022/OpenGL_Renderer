//
// Created by Tony Adriansen on 3/20/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSOR_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSOR_HPP_

#include "src/renderer/gl/FBOContainer.hpp"
#include "PostProcessingEffect.hpp"
#include "Quad.hpp"

class PostProcessor {
 public:
  explicit PostProcessor(const Quad& quad);
  void Init(uint32_t width, uint32_t height);
  void Resize(uint32_t width, uint32_t height);
  void Render(Texture* texture);

  struct Uniforms {
    float contrast = 0.0f;
  };

  Uniforms uniforms;

  void AssignShaders();
 private:
  const Quad& m_quad;
  std::vector<std::unique_ptr<PPEffect>> m_effects;
  Shader* contrastShader = nullptr;
  Shader* invertShader = nullptr;
  void LoadShaders();

  std::unique_ptr<FBOContainer> makeFBOContainer(uint32_t width, uint32_t height);
};

#endif //OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSOR_HPP_
