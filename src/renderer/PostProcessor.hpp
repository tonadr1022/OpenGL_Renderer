//
// Created by Tony Adriansen on 3/20/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSOR_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSOR_HPP_

#include "src/renderer/gl/FBOContainer.hpp"
#include "PostProcessingEffect.hpp"
#include "Quad.hpp"
#include <bitset>

class PostProcessor {
 public:
  struct Uniforms {
    float contrast = 0.3f;
    float grayscale = 0.0f;
    glm::vec4 colorChannels = {1.0f,1.0f,1.0f, 1.0f};
  };

  explicit PostProcessor(const Quad& quad);
  void Init(uint32_t width, uint32_t height);
  void Resize(uint32_t width, uint32_t height);
  void EnableEffect(PostProcessingEffectType type);
  void DisableEffect(PostProcessingEffectType type);
  void Render(Texture* texture);
  [[nodiscard]] inline const std::vector<Texture*>& GetResultTextures() const { return m_resultTextures; };
  void OnImGui();

  Uniforms uniforms;
 private:
  std::bitset<static_cast<size_t>(PostProcessingEffectType::Count)> m_effectEnabledBits;
  void EnabledCheckbox(int i);
  const Quad& m_quad;
  std::vector<PostProcessingEffect> m_effects;
  std::vector<Texture*> m_resultTextures;
  void LoadShaders();
  static std::unique_ptr<FBOContainer> makeFBOContainer(uint32_t width, uint32_t height, float resolutionScale = 1);

  static constexpr const char* ColorChannelToString(int i) {
    switch (i) {
      case 0: return "Red";
      case 1: return "Green";
      default: return "Blue";
    }

  }
};

#endif //OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSOR_HPP_
