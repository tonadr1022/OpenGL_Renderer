//
// Created by Tony Adriansen on 3/20/24.
//

#include "PostProcessor.hpp"
#include "src/renderer/resource/ShaderManager.hpp"

PostProcessor::PostProcessor(const Quad& quad) : m_quad(quad) {

}

void PostProcessor::Init(uint32_t width, uint32_t height) {
  m_effects.clear();

  contrastShader = ShaderManager::GetShader("contrast");
  invertShader = ShaderManager::GetShader("invert");

  auto contrastUniformSetter = [this]() {
    contrastShader->SetFloat("u_Contrast", uniforms.contrast);
  };

  m_effects.emplace_back(
      std::make_unique<PostProcessingEffect>("Contrast", contrastShader,
                                             makeFBOContainer(width, height), contrastUniformSetter));
  m_effects.emplace_back(std::make_unique<PostProcessingEffect>("Invert",
                                                                invertShader,
                                                                makeFBOContainer(width, height)));
//m_effects.emplace_back(std::make_unique<PostProcessingEffect>("Grayscale", "grayscale", makeFBOContainer(width, height)));

}

void PostProcessor::Resize(uint32_t width, uint32_t height) {
  for (auto& effect : m_effects) {
    effect->SetFBOContainer(makeFBOContainer(width, height));
  }
}

void PostProcessor::Render() {
  m_quad.VAO().Bind();
  int i = 0;
  for (auto& effect : m_effects) {
    if (i == m_effects.size() - 1) {
      FrameBuffer::BindDefault();
    } else {
      effect->GetFBOContainer()->FBO().Bind();
      effect->GetFBOContainer()->Textures()[0]->Bind();
    }
    effect->GetShader()->Bind();
    effect->SetUniforms();
    glClear(GL_COLOR_BUFFER_BIT);
    m_quad.Draw();
  }
}

std::unique_ptr<FBOContainer> PostProcessor::makeFBOContainer(uint32_t width, uint32_t height) {
  auto fboContainer = std::make_unique<FBOContainer>();
  fboContainer->FBO().Bind();
  auto tex = std::make_unique<Texture>(width, height);
  fboContainer->AttachColorBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(tex));
  return fboContainer;
}
