//
// Created by Tony Adriansen on 2/28/2024.
//

#include "Application.hpp"

Application::Application() {
  m_window = std::make_unique<Window>();
  m_window->Init();

  m_renderer = std::make_unique<Renderer>();

}

void Application::Run() {
  while (!m_window->ShouldClose()) {

    Input::Update();

    m_window->StartFrame();

    m_renderer->OnRender();

    m_window->EndFrame();
  }
}
