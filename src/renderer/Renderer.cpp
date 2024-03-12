//
// Created by Tony Adriansen on 2/27/2024.
//

#include "Renderer.hpp"
#include "imgui/imgui.h"
#include "src/core/Logger.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "src/renderer/resource/ShaderManager.hpp"

#include "src/Common.hpp"
#include "src/renderer/gl/FrameBuffer.hpp"
#include "FrameCapturer.hpp"

void Renderer::UpdateRenderState(const Object& object) {
  if (object.GetMaterial() != state.boundMaterial) {
    state.boundMaterial = object.GetMaterial();
    if (state.boundMaterial->shaderName != state.boundShaderName) {
      state.boundShaderName = state.boundMaterial->shaderName;
      state.boundShader = ShaderManager::GetShader(state.boundMaterial->shaderName);
      state.boundShader->Bind();
//      state.boundShader->SetMat4("u_View", m_camera->GetViewMatrix());
      state.boundShader->SetInt("renderMode", static_cast<int>(debugMode));
      state.boundShader->SetVec3("u_ViewPos", m_camera->GetPosition());
      state.boundShader->SetMat4("u_VP", m_camera->GetVPMatrix());
//      state.boundShader->SetMat4("u_Projection", m_camera->GetProjectionMatrix());
      if (mode == Mode::BlinnPhong) SetLightingUniforms();
    }
    // For now, this assumes only one type per material. will need to refactor if otherwise
    for (auto&& texture : state.boundMaterial->textures) {
      switch (texture->GetType()) {
        case Texture::Type::Diffuse:texture->Bind(GL_TEXTURE0);
          state.boundShader->SetBool("hasDiffuseMap", true);
          state.boundShader->SetInt("materialMaps.diffuseMap", 0);
          break;
        case Texture::Type::Specular:texture->Bind(GL_TEXTURE1);
          state.boundShader->SetBool("hasSpecularMap", true);
          state.boundShader->SetInt("materialMaps.specularMap", 1);
          break;
        case Texture::Type::Emission:texture->Bind(GL_TEXTURE2);
          state.boundShader->SetBool("hasEmissionMap", true);
          state.boundShader->SetInt("materialMaps.emissionMap", 2);
          break;
        default:break;
      }
    }
    state.boundShader->SetVec3("material.ambient", {1.0f, 0.5f, 0.31f});
    state.boundShader->SetVec3("material.diffuse", {1.0f, 0.5f, 0.31f});
    state.boundShader->SetVec3("material.specular", {0.5, 0.5, 0.5});
    state.boundShader->SetFloat("material.shininess", 32);
  }
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
  if (m_settings.renderToImGuiViewport) {
    m_frameCapturer.StartCapture();
  } else {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

void Renderer::EndFrame() {
  if (m_settings.renderToImGuiViewport) {
    m_frameCapturer.EndCapture();
  }
}

void Renderer::RenderGroup(const Group& group) {
  if (!group.GetVisible()) return;
  if (group.GetWireFrame() && !m_settings.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if (!group.GetWireFrame() && m_settings.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  for (auto&& object : group.GetObjects()) {
    auto mesh = object->GetMesh();
    UpdateRenderState(*object);
    mesh->GetVAO().Bind();
    auto& modelMatrix = object->transform.GetModelMatrix();
    state.boundShader->SetMat4("u_Model", modelMatrix);
//    state.boundShader->SetMat3("u_NormalMatrix", glm::inverse( modelMatrix), true);

    glDrawElements(GL_TRIANGLES, (GLsizei) mesh->NumIndices(), GL_UNSIGNED_INT, nullptr);
    stats.drawCalls++;
    stats.vertices += mesh->NumIndices();
    stats.indices += mesh->NumIndices();
  }
}

void Renderer::Init() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

}

void Renderer::RenderScene(const Scene& scene, Camera* camera) {
  m_camera = camera;
  StartFrame(scene);
  for (auto& group : scene.GetGroups()) {
    RenderGroup(*group);
  }
  EndFrame();
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

namespace { // detail

auto GenerateSpotLightStrings(int num) {
  std::vector<HashedString> ret;
  ret.reserve(num * 10);
  for (int i = 0; i < num; i++) {
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].base.color").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].base.ambientIntensity").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].base.diffuseIntensity").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].base.specularIntensity").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].position").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].direction").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].linear").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].quadratic").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].innerCutoff").c_str());
    ret.emplace_back(("spotlights[" + std::to_string(i) + "].outerCutoff").c_str());
  }
  return ret;
}

std::vector<HashedString> GeneratePointLightStrings(int num) {
  std::vector<HashedString> ret;
  ret.reserve(num * 7);
  for (int i = 0; i < num; i++) {
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].base.color").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].base.ambientIntensity").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].base.diffuseIntensity").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].base.specularIntensity").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].position").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].linear").c_str());
    ret.emplace_back(("pointLights[" + std::to_string(i) + "].quadratic").c_str());
  }
  return ret;
}

std::array<HashedString, 5> directionalLightStrings = {
    HashedString("directionalLight.base.color"),
    HashedString("directionalLight.base.ambientIntensity"),
    HashedString("directionalLight.base.diffuseIntensity"),
    HashedString("directionalLight.base.specularIntensity"),
    HashedString("directionalLight.direction"),
};

std::vector<HashedString> pointLightStrings = GeneratePointLightStrings(50);
std::vector<HashedString> spotLightStrings = GenerateSpotLightStrings(50);

} // namespace detail

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
      state.boundShader->SetVec3(pointLightStrings[7 * i], light->color);
      state.boundShader->SetFloat(pointLightStrings[7 * i + 1], light->ambientIntensity);
      state.boundShader->SetFloat(pointLightStrings[7 * i + 2], light->diffuseIntensity);
      state.boundShader->SetFloat(pointLightStrings[7 * i + 3], light->specularIntensity);
      state.boundShader->SetVec3(pointLightStrings[7 * i + 4], light->position);
      state.boundShader->SetFloat(pointLightStrings[7 * i + 5], light->linear);
      state.boundShader->SetFloat(pointLightStrings[7 * i + 6], light->quadratic);
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
      state.boundShader->SetVec3(spotLightStrings[10 * i], light->color);
      state.boundShader->SetFloat(spotLightStrings[10 * i + 1], light->ambientIntensity);
      state.boundShader->SetFloat(spotLightStrings[10 * i + 2], light->diffuseIntensity);
      state.boundShader->SetFloat(spotLightStrings[10 * i + 3], light->specularIntensity);
      state.boundShader->SetVec3(spotLightStrings[10 * i + 4], light->position);
      state.boundShader->SetVec3(spotLightStrings[10 * i + 5], light->direction);
      state.boundShader->SetFloat(spotLightStrings[10 * i + 6], light->linear);
      state.boundShader->SetFloat(spotLightStrings[10 * i + 7], light->quadratic);
      state.boundShader->SetFloat(spotLightStrings[10 * i + 8], glm::cos(glm::radians(light->angle - light->penumbra)));
      state.boundShader->SetFloat(spotLightStrings[10 * i + 9], glm::cos(glm::radians(light->angle)));
      i++;
    }
    state.boundShader->SetInt("numSpotLights", i);
  } else {
    state.boundShader->SetBool("spotLightEnabled", false);
  }
}
