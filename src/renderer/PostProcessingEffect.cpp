//
// Created by Tony Adriansen on 3/20/24.
//

#include "PostProcessingEffect.hpp"
#include "src/renderer/resource/ShaderManager.hpp"

PostProcessingEffect::PostProcessingEffect(HashedString name,
                                           Shader* shader,
                                           std::unique_ptr<FBOContainer> FBOContainer,
                                           const std::function<void()>& uniformFunction)
    : m_shader(shader), m_uniformFunction(uniformFunction), m_FBOContainer(std::move(FBOContainer)) {
}

PostProcessingEffect::PostProcessingEffect(HashedString name,
                                           Shader* shader,
                                           std::unique_ptr<FBOContainer> FBOContainer)
    : m_shader(shader), m_FBOContainer(std::move(FBOContainer)) {
}

void PostProcessingEffect::SetFBOContainer(std::unique_ptr<FBOContainer> FBOContainer) {
  m_FBOContainer = std::move(FBOContainer);
}

void PostProcessingEffect::SetUniforms() {
  if (m_uniformFunction) {
    m_uniformFunction();
  }
}
