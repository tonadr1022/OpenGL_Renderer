//
// Created by Tony Adriansen on 2/27/2024.
//

#include "Renderer.hpp"
#include "imgui/imgui.h"

#include "src/core/Logger.hpp"


void Renderer::OnRender() {
  ImGui::ShowDemoWindow();
  LOG_INFO("Test");

}
