//
// Created by Tony Adriansen on 3/20/24.
//

#include "PostProcessor.hpp"
#include "src/renderer/resource/ShaderManager.hpp"
#include "src/core/Logger.hpp"
#include <imgui/imgui.h>

PostProcessor::PostProcessor(const Quad& quad) : m_quad(quad) {

}

void PostProcessor::Init(uint32_t width, uint32_t height) {
  LoadShaders();

  auto contrastFunc = [this]() {
    auto shader = ShaderManager::GetShader("contrast");
    shader->Bind();
    shader->SetFloat("u_Contrast", uniforms.contrast);
  };
  auto invertFunc = []() { ShaderManager::GetShader("invert")->Bind(); };
  auto grayscaleFunc = []() { ShaderManager::GetShader("grayscale")->Bind(); };
  auto colorChannelFunc = [this]() {
    auto shader = ShaderManager::GetShader("colorChannel");
    shader->Bind();
    shader->SetVec4("u_ColorChannels", uniforms.colorChannels);
  };

  m_effects.emplace_back(PostProcessingEffectType::Contrast, makeFBOContainer(width, height), contrastFunc);
  m_effects.emplace_back(PostProcessingEffectType::Invert, makeFBOContainer(width, height), invertFunc);
  m_effects.emplace_back(PostProcessingEffectType::Grayscale, makeFBOContainer(width, height), grayscaleFunc);
  m_effects.emplace_back(PostProcessingEffectType::ColorChannel, makeFBOContainer(width, height), colorChannelFunc);
  m_resultTextures.reserve(m_effects.size() + 1);
  m_effectEnabledBits.reset();
}

void PostProcessor::Resize(uint32_t width, uint32_t height) {
  for (auto& effect : m_effects) {
    effect.fboContainer = makeFBOContainer(width, height, effect.resolutionScale);
  }
}

void PostProcessor::Render(Texture* texture) {
  m_resultTextures.clear();
  m_resultTextures.emplace_back(texture);
  m_quad.VAO().Bind();
  // no depth dest when drawing screen quads
  glDisable(GL_DEPTH_TEST);
  // iterate over post-processing effects
  for (auto& effect : m_effects) {
    if (!m_effectEnabledBits.test(static_cast<uint32_t>(effect.type))) continue;

    // bind an FBO to render to texture, and clear the color buffer
    effect.fboContainer->FBO().Bind();
    glClear(GL_COLOR_BUFFER_BIT);
    // bind the shader for the effect and set its uniforms. Bind the current texture (result of previous draw) and draw
    if (effect.bindAndUniformFunction != nullptr) effect.bindAndUniformFunction();
    m_resultTextures.back()->Bind();
    m_quad.Draw();
    // get the output of the render for input to the next effect or screen
    m_resultTextures.emplace_back(effect.fboContainer->Textures()[0].get());
  }
  // render to the screen: bind default framebuffer, clear color buffer, bind default screen quad shader, get the final result
  // draw the quad
  FrameBuffer::BindDefault();
  glClear(GL_COLOR_BUFFER_BIT);
  ShaderManager::GetShader("quad")->Bind();
  m_resultTextures.back()->Bind();
  m_quad.Draw();
}

void PostProcessor::EnableEffect(PostProcessingEffectType type) {
  for (auto& effect : m_effects) {
    if (effect.type == type) effect.enabled = true;
  }
}

void PostProcessor::DisableEffect(PostProcessingEffectType type) {
  for (auto& effect : m_effects) {
    if (effect.type == type) effect.enabled = false;
  }
}

std::unique_ptr<FBOContainer> PostProcessor::makeFBOContainer(uint32_t width, uint32_t height, float resolutionScale) {
  auto fboContainer = std::make_unique<FBOContainer>();
  fboContainer->FBO().Bind();
  auto tex = std::make_unique<Texture>(width * resolutionScale, height * resolutionScale);
  fboContainer->AttachColorBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(tex));
  return fboContainer;
}

void PostProcessor::LoadShaders() {
  ShaderManager::AddShader("contrast", {{GET_SHADER_PATH("quad.vert"), ShaderType::VERTEX},
                                        {GET_SHADER_PATH("postprocessing/contrast.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("invert", {{GET_SHADER_PATH("quad.vert"), ShaderType::VERTEX},
                                      {GET_SHADER_PATH("postprocessing/invert.frag"), ShaderType::FRAGMENT}});
  ShaderManager::AddShader("colorChannel", {{GET_SHADER_PATH("quad.vert"), ShaderType::VERTEX},
                                            {GET_SHADER_PATH("postprocessing/colorChannel.frag"),
                                             ShaderType::FRAGMENT}});
  ShaderManager::AddShader("grayscale", {{GET_SHADER_PATH("quad.vert"), ShaderType::VERTEX},
                                         {GET_SHADER_PATH("postprocessing/grayscale.frag"), ShaderType::FRAGMENT}});

  ShaderManager::AddShader("quad", {{GET_SHADER_PATH("quad.vert"), ShaderType::VERTEX},
                                    {GET_SHADER_PATH("quad.frag"), ShaderType::FRAGMENT}});
}

void PostProcessor::OnImGui() {
  for (int i =0; i < (int)PostProcessingEffectType::Count; i++) {
    if (i & 1) ImGui::SameLine();
    EnabledCheckbox(i);
  }
  ImGui::DragFloat("Contrast##contrastslider", &uniforms.contrast, 0.01, -1, 1);
  ImGui::DragFloat4("Color Channels", &uniforms.colorChannels.x, 0.01, 0, 1);
}

void PostProcessor::EnabledCheckbox(int i) {
  auto type = static_cast<PostProcessingEffectType>(i);
  bool enabled = m_effectEnabledBits.test(i);
  if (ImGui::Checkbox(PostProcessingEffectTypeToString(type), &enabled)) {
    if (enabled) m_effectEnabledBits.set(i);
    else m_effectEnabledBits.reset(i);
  }
}
