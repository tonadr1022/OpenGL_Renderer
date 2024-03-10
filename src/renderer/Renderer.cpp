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
    if (state.boundMaterial->shader != state.boundShader) {
      state.boundShader = state.boundMaterial->shader;
      state.boundShader->Bind();
//      state.boundShader->SetMat4("u_View", m_camera->GetViewMatrix());
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
  }
}

void Renderer::ResetStats() {
  memset(&stats, 0, sizeof(PerFrameStats));
}

void Renderer::StartFrame(const Scene& scene) {
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
  auto& lights = scene.GetLights();
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

void Renderer::SetLightingUniforms() {
  state.boundShader->SetInt("pointLight.type", 1);
  state.boundShader->SetVec3("pointLight.position", {1.0, 1.0, 1.0});
  state.boundShader->SetVec3("pointLight.diffuse", {0.5, 0.5, 0.5});
  state.boundShader->SetVec3("pointLight.ambient", {0.2, 0.2, 0.2});
  state.boundShader->SetVec3("pointLight.specular", {1.0, 1.0, 1.0});

  state.boundShader->SetInt("directionalLight.type", 0);
  state.boundShader->SetVec3("directionalLight.direction", {-0.2f, -1.0f, -0.3f});
  state.boundShader->SetVec3("directionalLight.diffuse", {0.5, 0.5, 0.5});
  state.boundShader->SetVec3("directionalLight.ambient", {0.2, 0.2, 0.2});
  state.boundShader->SetVec3("directionalLight.specular", {1.0, 1.0, 1.0});

  state.boundShader->SetVec3("material.ambient", {1.0f, 0.5f, 0.31f});
  state.boundShader->SetVec3("material.diffuse", {1.0f, 0.5f, 0.31f});
  state.boundShader->SetVec3("material.specular", {0.5, 0.5, 0.5});
  state.boundShader->SetFloat("material.shininess", 32);
}

void Renderer::SetLights(const std::vector<std::unique_ptr<Light>>& lights) {
  m_lights = &lights;
}