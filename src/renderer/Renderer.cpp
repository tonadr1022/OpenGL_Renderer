//
// Created by Tony Adriansen on 2/27/2024.
//

#include "Renderer.hpp"

#include "imgui/imgui.h"
#include "src/Common.hpp"
#include "src/gl/FrameBuffer.hpp"
#include "src/resource/ShaderManager.hpp"
#include "src/utils/Logger.hpp"
#include "src/utils/Utils.hpp"

namespace {
// detail
#define NUM_SPOT_PARAMS 9
#define NUM_POINT_PARAMS 6
#define NUM_DIRECTIONAL_PARAMS 5

std::vector<HashedString> GenerateSpotLightStrings(int num) {
  std::vector<HashedString> ret;
  ret.reserve(num * NUM_SPOT_PARAMS);
  for (int i = 0; i < num; i++) {
    ret.emplace_back(
        ("spotlights[" + std::to_string(i) + "].base.color").c_str());
    ret.emplace_back(
        ("spotlights[" + std::to_string(i) + "].base.ambientIntensity")
            .c_str());
    ret.emplace_back(
        ("spotlights[" + std::to_string(i) + "].base.diffuseIntensity")
            .c_str());
    ret.emplace_back(
        ("spotlights[" + std::to_string(i) + "].base.specularIntensity")
            .c_str());
    ret.emplace_back(
        ("spotlights[" + std::to_string(i) + "].position").c_str());
    ret.emplace_back(
        ("spotlights[" + std::to_string(i) + "].direction").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].radius").c_str());
    ret.emplace_back(
        ("spotlights[" + std::to_string(i) + "].innerCutoff").c_str());
    ret.emplace_back(
        ("spotlights[" + std::to_string(i) + "].outerCutoff").c_str());
  }
  return ret;
}

std::vector<HashedString> GeneratePointLightStrings(int num) {
  std::vector<HashedString> ret;
  ret.reserve(num * NUM_POINT_PARAMS);
  for (int i = 0; i < num; i++) {
    ret.emplace_back(
        ("pointLights[" + std::to_string(i) + "].base.color").c_str());
    ret.emplace_back(
        ("pointLights[" + std::to_string(i) + "].base.ambientIntensity")
            .c_str());
    ret.emplace_back(
        ("pointLights[" + std::to_string(i) + "].base.diffuseIntensity")
            .c_str());
    ret.emplace_back(
        ("pointLights[" + std::to_string(i) + "].base.specularIntensity")
            .c_str());
    ret.emplace_back(
        ("pointLights[" + std::to_string(i) + "].position").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].radius").c_str());
  }
  return ret;
}

std::array<HashedString, NUM_DIRECTIONAL_PARAMS> directional_light_strings = {
    HashedString("directionalLight.base.color"),
    HashedString("directionalLight.base.ambientIntensity"),
    HashedString("directionalLight.base.diffuseIntensity"),
    HashedString("directionalLight.base.specularIntensity"),
    HashedString("directionalLight.direction"),
};

std::vector<HashedString> point_light_strings = GeneratePointLightStrings(50);
std::vector<HashedString> spot_light_strings = GenerateSpotLightStrings(50);
}  // namespace

void Renderer::UpdateRenderState(const Object &object) {
  Material *mat = object.GetMaterial();
  GL_LOG_ERROR();
  if (state.boundShaderName != mat->shaderName) {
    GL_LOG_ERROR();
    state.boundShader = ShaderManager::GetShader(mat->shaderName);
    state.boundShaderName = mat->shaderName;
    GL_LOG_ERROR();

    state.boundShader->Bind();
    stats.numShaderBinds++;

    GL_LOG_ERROR();
    if (mat != state.boundMaterial) {
      state.boundMaterial = mat;
      GL_LOG_ERROR();
      stats.numMaterialSwitches++;
      SetBlinnPhongUniforms();
      GL_LOG_ERROR();
    }

    GL_LOG_ERROR();
    m_skyboxTexture->Bind(GL_TEXTURE4);
    GL_LOG_ERROR();
    state.boundShader->SetInt("renderMode", static_cast<int>(debugMode));
    GL_LOG_ERROR();
    state.boundShader->SetInt("skybox", 4);
    GL_LOG_ERROR();
    state.boundShader->SetBool("useBlinn", m_settings.useBlinn);
    GL_LOG_ERROR();
    state.boundShader->SetVec3("u_ViewPos", m_camera->GetPosition());
    GL_LOG_ERROR();
    state.boundShader->SetMat4("u_VP", m_camera->GetVPMatrix());
    GL_LOG_ERROR();
    SetLightingUniforms();
    GL_LOG_ERROR();
  }
  if (mat != state.boundMaterial) {
    state.boundMaterial = mat;
    stats.numMaterialSwitches++;
    GL_LOG_ERROR();
    SetBlinnPhongUniforms();
  }
  GL_LOG_ERROR();
}

void Renderer::SetBlinnPhongUniforms() {
  // For now, this assumes only one type per material. will need to refactor if
  // otherwise
  uint32_t num_diffuse_maps = 0;
  uint32_t num_specular_maps = 0;
  uint32_t num_emission_maps = 0;
  for (auto &&texture_pair : state.boundMaterial->textures) {
    switch (texture_pair.first) {
      case MatTextureType::Diffuse:
        texture_pair.second->Bind(GL_TEXTURE0);
        state.boundShader->SetInt("materialMaps.diffuseMap", 0);
        num_diffuse_maps++;
        break;
      case MatTextureType::Specular:
        texture_pair.second->Bind(GL_TEXTURE1);
        state.boundShader->SetInt("materialMaps.specularMap", 1);
        num_specular_maps++;
        break;
      case MatTextureType::Emission:
        texture_pair.second->Bind(GL_TEXTURE2);
        state.boundShader->SetInt("materialMaps.emissionMap", 2);
        num_emission_maps++;
        break;
      default:
        break;
    }
  }
  state.boundShader->SetBool(
      "hasDiffuseMap", m_settings.diffuseMapEnabled && num_diffuse_maps > 0);
  state.boundShader->SetBool(
      "hasSpecularMap", m_settings.specularMapEnabled && num_specular_maps > 0);
  state.boundShader->SetBool(
      "hasEmissionMap", m_settings.emissionMapEnabled && num_emission_maps > 0);

  state.boundShader->SetVec3("material.ambient",
                             state.boundMaterial->ambientColor);
  state.boundShader->SetVec3("material.diffuse",
                             state.boundMaterial->diffuseColor);
  state.boundShader->SetVec3("material.specular",
                             state.boundMaterial->specularColor);
  state.boundShader->SetFloat("material.shininess",
                              state.boundMaterial->shininess);
}

void Renderer::ResetStats() { stats = {}; }

void Renderer::StartFrame(const Scene & /*scene*/) {
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

void Renderer::RenderGroup(const Group &group) {
  if (!group.visible) return;
  if (group.wireframe && !m_settings.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if (!group.wireframe && m_settings.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if (!group.backFaceCull) {
    glDisable(GL_CULL_FACE);
  }

  for (auto &&object : group.GetObjects()) {
    if (!object->shouldDraw) continue;
    const auto *mesh = object->GetMesh();
    UpdateRenderState(*object);
    mesh->GetVAO().Bind();
    state.boundShader->SetMat4("u_Model", object->transform.GetModelMatrix());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->NumIndices()),
                   GL_UNSIGNED_INT, nullptr);
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
    for (auto &&object : group.GetObjects()) {
      if (!object->shouldDraw) continue;
      const auto *mesh = object->GetMesh();
      mesh->GetVAO().Bind();
      state.boundShader->SetMat4("u_Model", object->transform.GetModelMatrix());
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->NumIndices()),
                     GL_UNSIGNED_INT, nullptr);
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

  // action to take when any stencil test passes or fails, replace when pass
  // stencil test and depth test (or only stencil if no depth test)
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_NOTEQUAL, 1,
                0xFF);  // all fragments should pass stencil test
}

void Renderer::Reset() {}

void Renderer::RenderScene(const Scene &scene, Camera *camera) {
  GL_LOG_ERROR();
  m_camera = camera;
  StartFrame(scene);
  for (const auto &group : scene.GetGroups()) {
    RenderGroup(*group);
  }
  if (m_settings.renderSkybox) RenderSkybox(camera);

  // blit from multi-sampled result to the intermediate FBO
  glBindFramebuffer(GL_READ_FRAMEBUFFER,
                    m_settings.useMSAA
                        ? m_multiSampleFBOContainer->FBO().Id()
                        : m_singleSampleFBOContainer->FBO().Id());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                    m_resolveSampleFBOContainer->FBO().Id());
  glBlitFramebuffer(0, 0, static_cast<GLsizei>(m_width),
                    static_cast<GLsizei>(m_height), 0, 0,
                    static_cast<GLsizei>(m_width),
                    static_cast<GLsizei>(m_height), 0x00004000, GL_NEAREST);
  m_postProcessor.Render(m_resolveSampleFBOContainer->Textures()[0].get());
}

void Renderer::RenderSkybox(Camera *camera) {
  glDepthFunc(GL_LEQUAL);

  m_skyboxTexture->Bind(GL_TEXTURE0);

  m_skyboxShader->Bind();
  glm::mat4 vp = camera->GetProjectionMatrix() *
                 glm::mat4(glm::mat3(camera->GetViewMatrix()));
  m_skyboxShader->SetMat4("VP", vp);
  m_skybox.Draw();
  //  m_skyboxShader->Unbind();
  //  t->Unbind();
  glDepthFunc(GL_LESS);
  //  glEnable(GL_STENCIL_TEST);
}

void Renderer::ResizeViewport(uint32_t width, uint32_t height) {
  glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
  m_width = width;
  m_height = height;
  AllocateFBOContainers(width, height);
  m_postProcessor.Resize(width, height);
}

Renderer::Renderer(Window &window)
    : m_window(window), m_postProcessor(m_screenQuad) {
  auto frame_buffer_dims = window.GetFrameBufferDimensions();
  ResizeViewport(frame_buffer_dims.x, frame_buffer_dims.y);
  m_postProcessor.Init(frame_buffer_dims.x, frame_buffer_dims.y);
}

void Renderer::SetPointLights(
    const std::vector<std::unique_ptr<PointLight> > *pointLights) {
  m_pointLights = pointLights;
}

void Renderer::SetDirectionalLight(const DirectionalLight *directionalLight) {
  m_directionalLight = directionalLight;
}
void Renderer::SetSpotLights(
    const std::vector<std::unique_ptr<SpotLight> > *spotLights) {
  m_spotLights = spotLights;
}

void Renderer::SetLightingUniforms() {
  GL_LOG_ERROR();
  if (m_settings.renderDirectionalLights && m_directionalLight != nullptr) {
    state.boundShader->SetBool("directionalLightEnabled", true);
    state.boundShader->SetVec3(directional_light_strings[0],
                               m_directionalLight->color);
    state.boundShader->SetFloat(directional_light_strings[1],
                                m_directionalLight->ambientIntensity);
    state.boundShader->SetFloat(directional_light_strings[2],
                                m_directionalLight->diffuseIntensity);
    state.boundShader->SetFloat(directional_light_strings[3],
                                m_directionalLight->specularIntensity);
    state.boundShader->SetVec3(directional_light_strings[4],
                               m_directionalLight->direction);
    GL_LOG_ERROR();
  } else {
    GL_LOG_ERROR();
    state.boundShader->SetBool("directionalLightEnabled", false);
  }
  if (m_settings.renderPointLights && m_pointLights != nullptr) {
    state.boundShader->SetBool("pointLightEnabled", true);
    int i = 0;
    for (const auto &light : *m_pointLights) {
      GL_LOG_ERROR();
      state.boundShader->SetVec3(point_light_strings[NUM_POINT_PARAMS * i],
                                 light->color);
      state.boundShader->SetFloat(point_light_strings[NUM_POINT_PARAMS * i + 1],
                                  light->ambientIntensity);
      state.boundShader->SetFloat(point_light_strings[NUM_POINT_PARAMS * i + 2],
                                  light->diffuseIntensity);
      state.boundShader->SetFloat(point_light_strings[NUM_POINT_PARAMS * i + 3],
                                  light->specularIntensity);
      state.boundShader->SetVec3(point_light_strings[NUM_POINT_PARAMS * i + 4],
                                 light->position);
      state.boundShader->SetFloat(point_light_strings[NUM_POINT_PARAMS * i + 5],
                                  light->radius);
      i++;
    }
    state.boundShader->SetInt("numPointLights", i);
  } else {
    state.boundShader->SetBool("pointLightEnabled", false);
  }
  if (m_settings.renderSpotlights && m_spotLights != nullptr) {
    state.boundShader->SetBool("spotLightEnabled", true);
    int i = 0;
    for (const auto &light : *m_spotLights) {
      state.boundShader->SetVec3(spot_light_strings[NUM_SPOT_PARAMS * i],
                                 light->color);
      state.boundShader->SetFloat(spot_light_strings[NUM_SPOT_PARAMS * i + 1],
                                  light->ambientIntensity);
      state.boundShader->SetFloat(spot_light_strings[NUM_SPOT_PARAMS * i + 2],
                                  light->diffuseIntensity);
      state.boundShader->SetFloat(spot_light_strings[NUM_SPOT_PARAMS * i + 3],
                                  light->specularIntensity);
      state.boundShader->SetVec3(spot_light_strings[NUM_SPOT_PARAMS * i + 4],
                                 light->position);
      state.boundShader->SetVec3(spot_light_strings[NUM_SPOT_PARAMS * i + 5],
                                 light->direction);
      state.boundShader->SetFloat(spot_light_strings[NUM_SPOT_PARAMS * i + 6],
                                  light->radius);
      state.boundShader->SetFloat(
          spot_light_strings[NUM_SPOT_PARAMS * i + 7],
          glm::cos(glm::radians(light->angle - light->penumbra)));
      state.boundShader->SetFloat(spot_light_strings[NUM_SPOT_PARAMS * i + 8],
                                  glm::cos(glm::radians(light->angle)));
      i++;
    }
    GL_LOG_ERROR();
    state.boundShader->SetInt("numSpotLights", i);
  } else {
    GL_LOG_ERROR();
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

void Renderer::SetSkyboxTexture(Texture *texture) { m_skyboxTexture = texture; }

void Renderer::IncStats(uint32_t numVertices, uint32_t numIndices) {
  stats.drawCalls++;
  stats.vertices += numVertices;
  stats.indices += numIndices;
}

void Renderer::Screenshot(std::string_view filename) {
  if (std::isalpha(filename[0])) {
    const Texture &final_texture = GetFinalImageTexture();
    final_texture.Screenshot(m_width, m_height, std::string(filename) + ".png");
  } else {
    Screenshot();
  }
}

void Renderer::Screenshot() {
  std::string dt_string = Utils::GetDateTimeString();
  std::string screenshot_name = "screenshot_" + dt_string + ".png";
  const Texture &final_texture = GetFinalImageTexture();
  final_texture.Screenshot(m_width, m_height, screenshot_name);
}

const Texture &Renderer::GetFinalImageTexture() {
  return *m_postProcessor.GetResultTextures().back();
}

void Renderer::OnImGui() {
  if (ImGui::CollapsingHeader("Post Processing",
                              ImGuiTreeNodeFlags_DefaultOpen)) {
    m_postProcessor.OnImGui();
  }
}

void Renderer::AllocateFBOContainers(uint32_t width, uint32_t height) {
  // MSAA Framebuffer
  int samples = 4;
  m_multiSampleFBOContainer = std::make_unique<FBOContainer>();
  m_multiSampleFBOContainer->FBO().Bind();
  // attach color buffer
  auto multi_sample_texture = std::make_unique<Texture>(width, height, samples);
  m_multiSampleFBOContainer->AttachColorBuffer(GL_COLOR_ATTACHMENT0,
                                               GL_TEXTURE_2D_MULTISAMPLE,
                                               std::move(multi_sample_texture));
  // attach stencil and depth buffer
  auto multi_sample_rbo = std::make_unique<RenderBuffer>(GL_DEPTH24_STENCIL8);
  multi_sample_rbo->Bind();
  multi_sample_rbo->BufferStorageMultiSample(width, height, samples);
  m_multiSampleFBOContainer->AttachRenderBuffer(std::move(multi_sample_rbo));
  // FBO to blit store color on blit from MSAA framebuffer, contains only color
  // attachment

  // Single sample FBO
  m_singleSampleFBOContainer = std::make_unique<FBOContainer>();
  m_singleSampleFBOContainer->FBO().Bind();
  auto single_sample_texture = std::make_unique<Texture>(width, height);
  m_singleSampleFBOContainer->AttachColorBuffer(
      GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(single_sample_texture));
  auto single_sample_rbo = std::make_unique<RenderBuffer>(GL_DEPTH24_STENCIL8);
  single_sample_rbo->Bind();
  single_sample_rbo->BufferStorage(width, height);
  m_singleSampleFBOContainer->AttachRenderBuffer(std::move(single_sample_rbo));

  // Resolve FBO
  m_resolveSampleFBOContainer = std::make_unique<FBOContainer>();
  m_resolveSampleFBOContainer->FBO().Bind();
  auto resolve_sample_texture = std::make_unique<Texture>(width, height);
  m_resolveSampleFBOContainer->AttachColorBuffer(
      GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(resolve_sample_texture));
  auto resolve_sample_rbo = std::make_unique<RenderBuffer>(GL_DEPTH24_STENCIL8);
  resolve_sample_rbo->Bind();
  resolve_sample_rbo->BufferStorage(width, height);
  m_resolveSampleFBOContainer->AttachRenderBuffer(
      std::move(resolve_sample_rbo));
}
