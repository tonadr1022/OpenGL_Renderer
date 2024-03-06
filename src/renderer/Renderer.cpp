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
namespace Renderer {

namespace { // detail

struct State {
  const Material* boundMaterial = nullptr;
  Shader* boundShader = nullptr;
  Camera* activeCamera = nullptr;
  bool imguiFullScreen{true};
  int windowWidth{1}, windowHeight{1};
  bool wireframe{false};
};

std::unique_ptr<FrameCapturer> frameCapturer;
State state;
PerFrameStats stats;

void UpdateRenderState(const Object& object) {
  if (object.GetMaterial() != state.boundMaterial) {
    state.boundMaterial = object.GetMaterial();
    if (state.boundMaterial->shader != state.boundShader) {
      state.boundShader = state.boundMaterial->shader;
      state.boundShader->Bind();
      state.boundShader->SetMat4("u_VP", state.activeCamera->GetVPMatrix());
    }
    for (auto&& texture : state.boundMaterial->textures) {
      texture->Bind();
    }
  }
}

void ResetStats() {
  memset(&stats, 0, sizeof(PerFrameStats));
}

void StartFrame(const Scene& scene) {
  state.boundShader = nullptr;
  state.boundMaterial = nullptr;
  state.activeCamera = scene.GetCamera();
  ResetStats();
  state.activeCamera->SetAspectRatio(GetAspectRatio());
  glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);
  if (state.imguiFullScreen) {
    frameCapturer->StartCapture();
  } else {
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

void EndFrame() {
  if (state.imguiFullScreen) {
    frameCapturer->EndCapture();
  }
}

void RenderGroup(const Group& group) {
  if (!group.GetVisible()) return;
  if (group.GetWireFrame() && !state.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if (!group.GetWireFrame() && state.wireframe) {
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

void ImGuiDisplay() {
  ImGui::Begin("Stats");
  ImGui::Text("Draw Calls: %i", stats.drawCalls);
  ImGui::Text("Vertices: %i", stats.vertices);
  ImGui::Text("Indices: %i", stats.indices);
  ImGui::End();
  ImGui::Begin("Settings");
  ImGui::Checkbox("Wireframe", &state.wireframe);
  ImGui::End();

  if (state.imguiFullScreen) {
    ImGui::Begin("Viewport");
    ImGui::Image((void*) (intptr_t) frameCapturer->GetTexture().Id(), ImGui::GetContentRegionAvail());
    ImGui::End();
  }
}

} // namespace detail


void Init() {
  frameCapturer = std::make_unique<FrameCapturer>(state.windowWidth, state.windowHeight);
}

void RenderScene(const Scene& scene) {
  StartFrame(scene);

  for (auto& group : scene.GetGroups()) {
    RenderGroup(*group);
  }

  EndFrame();
  ImGuiDisplay();
}

void SetWindowSize(int width, int height) {
  state.windowWidth = width;
  state.windowHeight = height;
  glViewport(0, 0, width, height);
  if (frameCapturer)
    frameCapturer->UpdateViewport(width, height);
}

float GetAspectRatio() {
  return static_cast<float>(state.windowWidth) / static_cast<float>(state.windowHeight);
}

void SetImGuiFullScreen(bool imguiFullScreen) {
  state.imguiFullScreen = imguiFullScreen;
}

} // namespace Renderer
