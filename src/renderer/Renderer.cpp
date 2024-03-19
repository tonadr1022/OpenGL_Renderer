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
#include "FrameCapturer.hpp"

#define MAX_USED_TEXTURE_SLOTS 5
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
//    ret.emplace_back(("pointLights[" + std::to_string(i) + "].quadratic").c_str());
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
  if (object.GetMaterial() != state.boundMaterial) {
    state.boundMaterial = object.GetMaterial();
    if (state.boundMaterial->shaderName != state.boundShaderName) {
      state.boundShaderName = state.boundMaterial->shaderName;
      state.boundShader = ShaderManager::GetShader(state.boundMaterial->shaderName);
      state.boundShader->Bind();
//      state.boundShader->SetMat4("u_View", m_camera->GetViewMatrix());
      state.boundShader->SetInt("renderMode", static_cast<int>(debugMode));
      state.boundShader->SetBool("useBlinn", m_settings.useBlinn);
      state.boundShader->SetVec3("u_ViewPos", m_camera->GetPosition());
      state.boundShader->SetMat4("u_VP", m_camera->GetVPMatrix());
//      state.boundShader->SetMat4("u_Projection", m_camera->GetProjectionMatrix());

      if (mode == Mode::BlinnPhong) {
        SetBlinnPhongUniforms();
      }
      SetLightingUniforms();
    }
  }
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
      case MatTextureType::Specular:texturePair.second->Bind(GL_TEXTURE1);
        state.boundShader->SetInt("materialMaps.specularMap", 1);
        numSpecularMaps++;
        break;
      case MatTextureType::Emission:texturePair.second->Bind(GL_TEXTURE2);
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
  state.boundShader = nullptr;
  state.boundMaterial = nullptr;
  state.boundShaderName = "";
  ResetStats();
  glPolygonMode(GL_FRONT_AND_BACK, m_settings.wireframe ? GL_LINE : GL_FILL);
//  if (m_settings.renderToImGuiViewport) {
  m_frameCapturer.StartCapture();
//  } else {
//    glClearColor(0, 0, 0, 1);
//    glEnable(GL_DEPTH_TEST);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  }
}

void Renderer::RenderGroup(const Group& group) {
  if (!group.GetVisible()) return;
  if (group.GetWireFrame() && !m_settings.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if (!group.GetWireFrame() && m_settings.wireframe) {
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
    auto& modelMatrix = object->transform.GetModelMatrix();
    state.boundShader->SetMat4("u_Model", modelMatrix);
//    state.boundShader->SetMat3("u_NormalMatrix", glm::inverse( modelMatrix), true);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh->NumIndices(), GL_UNSIGNED_INT, nullptr);

    stats.drawCalls++;
    stats.vertices += mesh->NumVertices();
    stats.indices += mesh->NumIndices();
  }

  if (!group.backFaceCull) {
    glEnable(GL_CULL_FACE);
  }
}

void Renderer::Init() {
  ShaderManager::AddShader("screen1", {{GET_SHADER_PATH("contrast.vert"), ShaderType::VERTEX},
                                       {GET_SHADER_PATH("contrast.frag"), ShaderType::FRAGMENT}});
  AssignShaders();
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void Renderer::Reset() {

}

void Renderer::RenderScene(const Scene& scene, Camera* camera) {
  m_camera = camera;
  StartFrame(scene);
  for (auto& group : scene.GetGroups()) {
    RenderGroup(*group);
  }
  RenderSkybox(camera);

  m_frameCapturer.EndCapture();

  glDisable(GL_DEPTH_TEST);
  m_screenShader->Bind();
  m_frameCapturer.GetTexture().Bind(GL_TEXTURE0);
  m_screenQuad.Draw();
}

void Renderer::RenderSkybox(Camera* camera) {
  glDepthFunc(GL_LEQUAL);
  m_skyboxShader->Bind();
  Texture* t = TextureManager::GetTexture("skybox");
  t->Bind(GL_TEXTURE0);
  glm::mat4 vp = camera->GetProjectionMatrix() * glm::mat4(glm::mat3(camera->GetViewMatrix()));
  m_skyboxShader->SetMat4("VP", vp);
  m_skyboxShader->SetInt("tex", 0);
  // hacky for now, will hopefully assert or use references for skybox texture
  m_skybox.Draw();
  glDepthFunc(GL_LESS);
}

void Renderer::ApplyPostProcessingEffects() {

}

void Renderer::SetFrameBufferSize(uint32_t width, uint32_t height) {
  glViewport(0, 0, (int) width, (int) height);
  m_frameCapturer.UpdateViewport(width, height);
}

Renderer::Renderer(Window& window, bool renderToImGuiViewport)
    : m_window(window), m_frameCapturer(window.GetFrameBufferDimensions().x, m_window.GetFrameBufferDimensions().y) {
  m_settings.renderToImGuiViewport = renderToImGuiViewport;
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
      state.boundShader->SetFloat(spotLightStrings[NUM_SPOT_PARAMS * i + 8],
                                  glm::cos(glm::radians(light->angle - light->penumbra)));
      state.boundShader->SetFloat(spotLightStrings[NUM_SPOT_PARAMS * i + 9], glm::cos(glm::radians(light->angle)));
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
  m_screenShader = ShaderManager::GetShader("screen1");
  m_skyboxShader = ShaderManager::GetShader("skybox");
}
