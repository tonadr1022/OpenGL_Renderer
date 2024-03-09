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
      state.boundShader->SetMat4("u_VP", m_camera.GetVPMatrix());
    }
    for (auto&& texture : state.boundMaterial->textures) {
      texture->Bind();
    }
  }
}

void Renderer::ResetStats() {
  memset(&stats, 0, sizeof(PerFrameStats));
}

void Renderer::StartFrame(const Scene& scene) {
  state.boundShader = nullptr;
  state.boundMaterial = nullptr;
  ResetStats();
  glPolygonMode(GL_FRONT_AND_BACK, m_settings.wireframe ? GL_LINE : GL_FILL);
  if (m_settings.renderToImGuiViewport) {
    m_frameCapturer.StartCapture();
  } else {
    glClearColor(0.5, 0, 0, 1);
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
    state.boundShader->SetMat4("u_Model", object->transform.GetModelMatrix());
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

void Renderer::RenderScene(const Scene& scene, const Camera& camera) {
  m_camera = camera;
  StartFrame(scene);

  for (auto& group : scene.GetGroups()) {
    RenderGroup(*group);
  }
  EndFrame();
}

void Renderer::SetWindowSize(uint32_t width, uint32_t height) {
  glViewport(0, 0, (int) width, (int) height);
  m_frameCapturer.UpdateViewport(width, height);
}

Renderer::Renderer(Window& window, Camera& camera, bool renderToImGuiViewport)
    : m_window(window), m_camera(camera), m_frameCapturer(window.GetWidth(), window.GetHeight()) {
  m_settings.renderToImGuiViewport = renderToImGuiViewport;
}

