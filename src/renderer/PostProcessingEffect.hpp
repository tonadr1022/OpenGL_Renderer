//
// Created by Tony Adriansen on 3/20/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSINGEFFECT_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSINGEFFECT_HPP_

#include "src/renderer/gl/FBOContainer.hpp"
#include "src/renderer/gl/Shader.hpp"

enum class PostProcessingEffectType {
  Contrast = 0, Invert, Grayscale, ColorChannel, GaussianBlur, Count
};

static constexpr const char* PostProcessingEffectTypeToString(PostProcessingEffectType type) {
  switch (type) {
    case PostProcessingEffectType::Contrast: return "Contrast";
    case PostProcessingEffectType::Invert: return "Invert";
    case PostProcessingEffectType::Grayscale: return "Grayscale";
    case PostProcessingEffectType::GaussianBlur: return "Gaussian Blue";
    case PostProcessingEffectType::ColorChannel: return "Color Channel";
    default: return "None";
  }
}

struct PostProcessingEffect {
  PostProcessingEffectType type;
  bool enabled = true;
  std::function<void()> bindAndUniformFunction = nullptr;
  std::unique_ptr<FBOContainer> fboContainer = nullptr;
  float resolutionScale;

  PostProcessingEffect(PostProcessingEffectType type,
                       std::unique_ptr<FBOContainer> fboContainer,
                       const std::function<void()>& bindAndUniformFunction,
                       float resolutionScale = 1)
      : type(type),
        enabled(true),
        bindAndUniformFunction(bindAndUniformFunction),
        fboContainer(std::move(fboContainer)),
        resolutionScale(resolutionScale) {}

  PostProcessingEffect(PostProcessingEffectType type,
                       std::unique_ptr<FBOContainer> fboContainer,
                       float resolutionScale = 1)
      : type(type), enabled(true), fboContainer(std::move(fboContainer)), resolutionScale(resolutionScale) {}
};

#endif //OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSINGEFFECT_HPP_
