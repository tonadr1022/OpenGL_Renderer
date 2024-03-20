//
// Created by Tony Adriansen on 2/27/2024.
//

#include "Renderer.hpp"
#include "imgui/imgui.h"
#include "src/core/Logger.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "src/renderer/resource/ShaderManager.hpp"
#include "src/renderer/resource/TextureManager.hpp"

#include "src/Common.hpp"
#include "src/renderer/gl/FrameBuffer.hpp"
#include "src/core/Utils.hpp"

#include <chrono>

namespace { // detail
#define NUM_SPOT_PARAMS 9
#define NUM_POINT_PARAMS 6
#define NUM_DIRECTIONAL_PARAMS 5

std::vector<HashedString> GenerateSpotLightStrings(int num) {
  std::vector<HashedString> ret;
  ret.reserve(num * NUM_SPOT_PARAMS);
  for (int i = 0; i < num; i++) {
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].base.color").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].base.ambientIntensity").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].base.diffuseIntensity").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].base.specularIntensity").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].position").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].direction").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].radius").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].innerCutoff").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].outerCutoff").c_str());
  }
  return ret;
}

std::vector<HashedString> GeneratePointLightStrings(int num) {
  std::vector<HashedString> ret;
  ret.reserve(num * NUM_POINT_PARAMS);
  for (int i = 0; i < num; i++) {
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].base.color").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].base.ambientIntensity").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].base.diffuseIntensity").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].base.specularIntensity").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].position").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].radius").c_str());
  }
  return ret;
}

std::array<HashedString, NUM_DIRECTIONAL_PARAMS> directionalLightStrings = {
    HashedString("directionalLight.base.color"),
    HashedString("directionalLight.base.ambientIntensity"),
    HashedString("directionalLight.base.diffuseIntensity"),
    HashedString("directionalLight.base.specularIntensity"),
    HashedString("directionalLight.direction"),
};

std::vector<HashedString> pointLightStrings = GeneratePointLightStrings(50);
std::vector<HashedString> spotLightStrings = GenerateSpotLightStrings(50);

} // namespace detail


void Renderer::UpdateRenderState(const Object& object) {
  Material* mat = object.GetMaterial();
  GL_LOG_ERROR();
  if (state.boundShaderName != mat->shaderName) {
    state.boundShader = ShaderManager::GetShader(mat->shaderName);
    state.boundShaderName = mat->shaderName;

    state.boundShader->Bind();
    stats.numShaderBinds++;

    if (mat != state.boundMaterial) {
      state.boundMaterial = mat;
      stats.numMaterialSwitches++;
      SetBlinnPhongUniforms();
      GL_LOG_ERROR();
    }

    GL_LOG_ERROR();
    m_skyboxTexture->Bind(GL_TEXTURE4);
    GL_LOG_ERROR();
    state.boundShader->SetInt("renderMode", static_cast<int>(debugMode));
    state.boundShader->SetInt("skybox", 4);
    state.boundShader->SetBool("useBlinn", m_settings.useBlinn);
    state.boundShader->SetVec3("u_ViewPos", m_camera->GetPosition());
    state.boundShader->SetMat4("u_VP", m_camera->GetVPMatrix());
    SetLightingUniforms();
  }
  if (mat != state.boundMaterial) {
    state.boundMaterial = mat;
    stats.numMaterialSwitches++;
    SetBlinnPhongUniforms();
  }
  GL_LOG_ERROR();
}

void Renderer::SetBlinnPhongUniforms() {
  // For now, this assumes only one type per material. will need to refactor if otherwise
  uint32_t numDiffuseMaps = 0, numSpecularMaps = 0, numEmissionMaps = 0;
  for (auto&& texturePair : state.boundMaterial->textures) {
    switch (texturePair.first) {
      case MatTextureType::Diffuse: texturePair.second->Bind(GL_TEXTURE0);
        state.boundShader->SetInt("materialMaps.diffuseMap", 0);
        numDiffuseMaps++;
        break;
      case MatTextureType::Specular: texturePair.second->Bind(GL_TEXTURE1);
        state.boundShader->SetInt("materialMaps.specularMap", 1);
        numSpecularMaps++;
        break;
      case MatTextureType::Emission: texturePair.second->Bind(GL_TEXTURE2);
        state.boundShader->SetInt("materialMaps.emissionMap", 2);
        numEmissionMaps++;
        break;
      default:break;
    }
  }
  state.boundShader->SetBool("hasDiffuseMap", m_settings.diffuseMapEnabled && numDiffuseMaps > 0);
  state.boundShader->SetBool("hasSpecularMap", m_settings.specularMapEnabled && numSpecularMaps > 0);
  state.boundShader->SetBool("hasEmissionMap", m_settings.emissionMapEnabled && numEmissionMaps > 0);

  state.boundShader->SetVec3("material.ambient", state.boundMaterial->ambientColor);
  state.boundShader->SetVec3("material.diffuse", state.boundMaterial->diffuseColor);
  state.boundShader->SetVec3("material.specular", state.boundMaterial->specularColor);
  state.boundShader->SetFloat("material.shininess", state.boundMaterial->shininess);
}

void Renderer::ResetStats() {
  memset(&stats, 0, sizeof(PerFrameStats));
}

void Renderer::StartFrame(const Scene& scene) {
  // reset state
  state.boundShader = nullptr;
  state.boundMaterial = nullptr;
  state.boundShaderName = "";
  ResetStats();
  glPolygonMode(GL_FRONT_AND_BACK, m_settings.wireframe ? GL_LINE : GL_FILL);

  if (m_settings.useMSAA) {
    m_multiSampleFBOContainer->FBO().Bind();
  } else {
    m_singleSampleFBOContainer->FBO().Bind();
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::RenderGroup(const Group& group) {
  if (!group.visible) return;
  if (group.wireframe && !m_settings.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if (!group.wireframe && m_settings.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if (!group.backFaceCull) {
    glDisable(GL_CULL_FACE);
  }

  for (auto&& object : group.GetObjects()) {
    if (!object->shouldDraw) continue;
    auto mesh = object->GetMesh();
    UpdateRenderState(*object);
    mesh->GetVAO().Bind();
    state.boundShader->SetMat4("u_Model", object->transform.GetModelMatrix());
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh->NumIndices(), GL_UNSIGNED_INT, nullptr);
    IncStats(mesh->NumVertices(), mesh->NumIndices());
  }

  if (group.selected) {
    // if selected, render twice, once as normal, writing to the stencil buffer,
    // then enlarged, disabling stencil wiring using the border color
    m_singleColorShader->Bind();
    state.boundShader = m_singleColorShader;
    state.boundShaderName = "";

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    m_singleColorShader->SetMat4("u_VP", m_camera->GetVPMatrix());
    for (auto&& object : group.GetObjects()) {
      if (!object->shouldDraw) continue;
      auto mesh = object->GetMesh();
      mesh->GetVAO().Bind();
      state.boundShader->SetMat4("u_Model", object->transform.GetModelMatrix());
      glDrawElements(GL_TRIANGLES, (GLsizei) mesh->NumIndices(), GL_UNSIGNED_INT, nullptr);
      IncStats(mesh->NumVertices(), mesh->NumIndices());
    }
    // reset stencil buffer state
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
  }

  if (!group.backFaceCull) {
    glEnable(GL_CULL_FACE);
  }

}

void Renderer::Init() {
  AssignShaders();
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // action to take when any stencil test passes or fails, replace when pass stencil
  // test and depth test (or only stencil if no depth test)
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // all fragments should pass stencil test
}

void Renderer::Reset() {

}

void Renderer::RenderScene(const Scene& scene, Camera* camera) {
  GL_LOG_ERROR();
  m_camera = camera;
  StartFrame(scene);
  for (auto& group : scene.GetGroups()) {
    RenderGroup(*group);
  }
  if (m_settings.renderSkybox) RenderSkybox(camera);

  // blit from multi-sampled result to the intermediate FBO
  glBindFramebuffer(GL_READ_FRAMEBUFFER,
                    m_settings.useMSAA ? m_multiSampleFBOContainer->FBO().Id()
                                       : m_singleSampleFBOContainer->FBO().Id());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_contrastFBOContainer->FBO().Id());
  glBlitFramebuffer(0, 0, (GLsizei) m_width, (GLsizei) m_height, 0, 0, (GLsizei) m_width,
                    (GLsizei) m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  if (ppUniforms.invert) {
    m_invertFBO->FBO().Bind();
  } else {
    FrameBuffer::BindDefault();
  }

  GL_LOG_ERROR();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_contrastShader->Bind();
  m_contrastShader->SetFloat("u_Contrast", ppUniforms.contrast);
  m_contrastFBOContainer->Textures()[0]->Bind();
  m_screenQuad.Draw();

  GL_LOG_ERROR();
  if (ppUniforms.invert) {
    FrameBuffer::BindDefault();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_invertShader->Bind();
    m_invertFBO->Textures()[0]->Bind();
    m_screenQuad.Draw();
    GL_LOG_ERROR();
  }
}

void Renderer::RenderSkybox(Camera* camera) {
  glDepthFunc(GL_LEQUAL);
//  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Keep stencil values unchanged
//  glDisable(GL_STENCIL_TEST);
//  glStencilFunc(GL_EQUAL, 1, 0xFF); // Only pass where stencil value is 1

  m_skyboxTexture->Bind(GL_TEXTURE0);

//  t->Bind(GL_TEXTURE0);
  m_skyboxShader->Bind();
  glm::mat4 vp = camera->GetProjectionMatrix() * glm::mat4(glm::mat3(camera->GetViewMatrix()));
  m_skyboxShader->SetMat4("VP", vp);
  m_skybox.Draw();
//  m_skyboxShader->Unbind();
//  t->Unbind();
  glDepthFunc(GL_LESS);
//  glEnable(GL_STENCIL_TEST);

}

void Renderer::ApplyPostProcessingEffects() {

}

void Renderer::ResizeViewport(uint32_t width, uint32_t height) {
  glViewport(0, 0, (int) width, (int) height);
  m_width = width;
  m_height = height;
  AllocateFBOContainers(width, height);
}

Renderer::Renderer(Window& window)
    : m_window(window), m_postProcessor(m_screenQuad) {
  auto frameBufferDims = window.GetFrameBufferDimensions();
  ResizeViewport(frameBufferDims.x, frameBufferDims.y);
}

void Renderer::SetPointLights(const std::vector<std::unique_ptr<PointLight>>* pointLights) {
  m_pointLights = pointLights;
}

void Renderer::SetDirectionalLight(const DirectionalLight* directionalLight) {
  m_directionalLight = directionalLight;
}
void Renderer::SetSpotLights(const std::vector<std::unique_ptr<SpotLight>>* spotLights) {
  m_spotLights = spotLights;
}

void Renderer::SetLightingUniforms() {
  if (m_settings.renderDirectionalLights && m_directionalLight != nullptr) {
    state.boundShader->SetBool("directionalLightEnabled", true);
    state.boundShader->SetVec3(directionalLightStrings[0], m_directionalLight->color);
    state.boundShader->SetFloat(directionalLightStrings[1], m_directionalLight->ambientIntensity);
    state.boundShader->SetFloat(directionalLightStrings[2], m_directionalLight->diffuseIntensity);
    state.boundShader->SetFloat(directionalLightStrings[3], m_directionalLight->specularIntensity);
    state.boundShader->SetVec3(directionalLightStrings[4], m_directionalLight->direction);
  } else {
    state.boundShader->SetBool("directionalLightEnabled", false);
  }
  if (m_settings.renderPointLights && m_pointLights != nullptr) {
    state.boundShader->SetBool("pointLightEnabled", true);
    int i = 0;
    for (auto& light : *m_pointLights) {
      state.boundShader->SetVec3(pointLightStrings[NUM_POINT_PARAMS * i], light->color);
      state.boundShader->SetFloat(pointLightStrings[NUM_POINT_PARAMS * i + 1], light->ambientIntensity);
      state.boundShader->SetFloat(pointLightStrings[NUM_POINT_PARAMS * i + 2], light->diffuseIntensity);
      state.boundShader->SetFloat(pointLightStrings[NUM_POINT_PARAMS * i + 3], light->specularIntensity);
      state.boundShader->SetVec3(pointLightStrings[NUM_POINT_PARAMS * i + 4], light->position);
      state.boundShader->SetFloat(pointLightStrings[NUM_POINT_PARAMS * i + 5], light->radius);
      i++;
    }
    state.boundShader->SetInt("numPointLights", i);
  } else {
    state.boundShader->SetBool("pointLightEnabled", false);
  }
  if (m_settings.renderSpotlights && m_spotLights != nullptr) {
    state.boundShader->SetBool("spotLightEnabled", true);
    int i = 0;
    for (auto& light : *m_spotLights) {
      state.boundShader->SetVec3(spotLightStrings[NUM_SPOT_PARAMS * i], light->color);
      state.boundShader->SetFloat(spotLightStrings[NUM_SPOT_PARAMS * i + 1], light->ambientIntensity);
      state.boundShader->SetFloat(spotLightStrings[NUM_SPOT_PARAMS * i + 2], light->diffuseIntensity);
      state.boundShader->SetFloat(spotLightStrings[NUM_SPOT_PARAMS * i + 3], light->specularIntensity);
      state.boundShader->SetVec3(spotLightStrings[NUM_SPOT_PARAMS * i + 4], light->position);
      state.boundShader->SetVec3(spotLightStrings[NUM_SPOT_PARAMS * i + 5], light->direction);
      state.boundShader->SetFloat(spotLightStrings[NUM_SPOT_PARAMS * i + 6], light->radius);
      state.boundShader->SetFloat(spotLightStrings[NUM_SPOT_PARAMS * i + 7],
                                  glm::cos(glm::radians(light->angle - light->penumbra)));
      state.boundShader->SetFloat(spotLightStrings[NUM_SPOT_PARAMS * i + 8], glm::cos(glm::radians(light->angle)));
      i++;
    }
    state.boundShader->SetInt("numSpotLights", i);
  } else {
    state.boundShader->SetBool("spotLightEnabled", false);
  }
}

void Renderer::RecompileShaders() {
  ShaderManager::RecompileShaders();
  AssignShaders();
}

void Renderer::AssignShaders() {
  m_contrastShader = ShaderManager::GetShader("contrast");
  m_invertShader = ShaderManager::GetShader("invert");
  m_skyboxShader = ShaderManager::GetShader("skybox");
  m_singleColorShader = ShaderManager::GetShader("singleColor");
  m_skyboxShader->Bind();
  m_skyboxShader->SetInt("skybox", 0);
}

void Renderer::SetSkyboxTexture(Texture* texture) {
  m_skyboxTexture = texture;
}

void Renderer::IncStats(uint32_t numVertices, uint32_t numIndices) {
  stats.drawCalls++;
  stats.vertices += numVertices;
  stats.indices += numIndices;
}

void Renderer::Screenshot(std::string_view filename) {
  if (std::isalpha(filename[0])) {
    const Texture& finalTexture = GetFinalImageTexture();
    finalTexture.Screenshot(m_width, m_height, std::string(filename) + ".png");
  } else {
    Screenshot();
  }
}

void Renderer::Screenshot() {
  std::string dtString = Utils::GetDateTimeString();
  std::string screenshotName = "screenshot_" + dtString + ".png";
  const Texture& finalTexture = GetFinalImageTexture();
  finalTexture.Screenshot(m_width, m_height, screenshotName);
}

const Texture& Renderer::GetFinalImageTexture() {
  ASSERT(!m_contrastFBOContainer->Textures().empty(), "Unexpected error, no textures attached to final framebuffer");
  return *m_contrastFBOContainer->Textures()[0];
}
void Renderer::AllocateFBOContainers(uint32_t width, uint32_t height) {
  // MSAA Framebuffer
  int samples = 4;
  m_multiSampleFBOContainer = std::make_unique<FBOContainer>();
  m_multiSampleFBOContainer->FBO().Bind();
  // attach color buffer
  auto multiSampleTexture = std::make_unique<Texture>(width, height, samples);
  m_multiSampleFBOContainer->AttachColorBuffer(GL_COLOR_ATTACHMENT0,
                                               GL_TEXTURE_2D_MULTISAMPLE,
                                               std::move(multiSampleTexture));
  // attach stencil and depth buffer
  auto multiSampleRBO = std::make_unique<RenderBuffer>(GL_DEPTH24_STENCIL8);
  multiSampleRBO->Bind();
  multiSampleRBO->BufferStorageMultiSample(width, height, samples);
  m_multiSampleFBOContainer->AttachRenderBuffer(std::move(multiSampleRBO));
  // FBO to blit store color on blit from MSAA framebuffer, contains only color attachment

  // Single sample FBO
  m_singleSampleFBOContainer = std::make_unique<FBOContainer>();
  m_singleSampleFBOContainer->FBO().Bind();
  auto singleSampleTexture = std::make_unique<Texture>(width, height);
  m_singleSampleFBOContainer->AttachColorBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(singleSampleTexture));
  auto singleSampleRBO = std::make_unique<RenderBuffer>(GL_DEPTH24_STENCIL8);
  singleSampleRBO->Bind();
  singleSampleRBO->BufferStorage(width, height);
  m_singleSampleFBOContainer->AttachRenderBuffer(std::move(singleSampleRBO));

  m_invertFBO = std::make_unique<FBOContainer>();
  m_invertFBO->FBO().Bind();
  auto invertTexture = std::make_unique<Texture>(width, height);
  m_invertFBO->AttachColorBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(invertTexture));

  m_contrastFBOContainer = std::make_unique<FBOContainer>();
  m_contrastFBOContainer->FBO().Bind();
  auto tex1 = std::make_unique<Texture>(width, height);
  m_contrastFBOContainer->AttachColorBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(tex1));

//  if (!m_contrastFBOContainer->FBO().IsComplete()) {
//    LOG_ERROR("contrast fbo incomplete");
//  }
//  if (!m_singleSampleFBOContainer->FBO().IsComplete()) {
//    LOG_ERROR("single sample fbo incomplete");
//  }
//  if (!m_multiSampleFBOContainer->FBO().IsComplete()) {
//    LOG_ERROR("multi sample fbo incomplete");
//  }
//  if (!m_invertFBO->FBO().IsComplete()) {
//    LOG_ERROR("invert fbo incomplete");
//  }
}
