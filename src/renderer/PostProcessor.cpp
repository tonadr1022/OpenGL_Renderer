//
// Created by Tony Adriansen on 3/20/24.
//

#include "PostProcessor.hpp"
#include "src/renderer/resource/ShaderManager.hpp"
#include "src/core/Logger.hpp"

PostProcessor::PostProcessor(const Quad& quad) : m_quad(quad) {

}

void PostProcessor::Init(uint32_t width, uint32_t height) {
  LoadShaders();
  AssignShaders();

  auto contrastUniformSetter = [this]() {
    contrastShader->SetFloat("u_Contrast", uniforms.contrast);
  };

  m_effects.emplace_back(
      std::make_unique<PPEffect>("Contrast", contrastShader, makeFBOContainer(width, height), contrastUniformSetter));
  m_effects.emplace_back(std::make_unique<PPEffect>("Invert", invertShader, makeFBOContainer(width, height)));
//m_effects.emplace_back(std::make_unique<PostProcessingEffect>("Grayscale", "grayscale", makeFBOContainer(width, height)));

}

void PostProcessor::Resize(uint32_t width, uint32_t height) {
  for (auto& effect : m_effects) {
    effect->fboContainer = makeFBOContainer(width, height);
  }
}

void PostProcessor::Render(Texture* texture) {
  m_quad.VAO().Bind();
  Texture* currTexture = texture;
  // no depth dest when drawing screen quads
  glDisable(GL_DEPTH_TEST);
  // iterate over post-processing effects
  for (auto& effect : m_effects) {
    // bind an FBO to render to texture, and clear the color buffer
    effect->fboContainer->FBO().Bind();
    glClear(GL_COLOR_BUFFER_BIT);
    // bind the shader for the effect and set its uniforms. Bind the current texture (result of previous draw) and draw
    effect->shader->Bind();
    if (effect->uniformFunction != nullptr) effect->uniformFunction();
    currTexture->Bind();
    m_quad.Draw();
    // get the output of the render for input to the next effect or screen
    currTexture = effect->fboContainer->Textures()[0].get();
  }
  // render to the screen: bind default framebuffer, clear color buffer, bind default screen quad shader, get the final result
  // draw the quad
  FrameBuffer::BindDefault();
  glClear(GL_COLOR_BUFFER_BIT);
  contrastShader->Bind();
  contrastShader->SetFloat("u_Contrast", 0.0);
  m_effects[m_effects.size() - 1]->fboContainer->Textures()[0]->Bind();
  m_quad.Draw();

}

std::unique_ptr<FBOContainer> PostProcessor::makeFBOContainer(uint32_t width, uint32_t height) {
  auto fboContainer = std::make_unique<FBOContainer>();
  fboContainer->FBO().Bind();
  auto tex = std::make_unique<Texture>(width, height);
  fboContainer->AttachColorBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(tex));
  return fboContainer;
}

void PostProcessor::LoadShaders() {
  ShaderManager::AddShader("contrast", {{GET_SHADER_PATH("contrast.vert"), ShaderType::VERTEX},
                                        {GET_SHADER_PATH("contrast.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("invert", {{GET_SHADER_PATH("postprocessing/invert.vert"), ShaderType::VERTEX},
                                      {GET_SHADER_PATH("postprocessing/invert.frag"), ShaderType::FRAGMENT}});
}

void PostProcessor::AssignShaders() {
  contrastShader = ShaderManager::GetShader("contrast");
  invertShader = ShaderManager::GetShader("invert");
}
