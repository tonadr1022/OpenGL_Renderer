//
// Created by Tony Adriansen on 2/27/2024.
//

#include "Window.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "src/imgui/ImguiStyles.h"

#include "src/core/Logger.hpp"
#include "src/renderer/Renderer.hpp"
#include "Input.hpp"

void Window::Init() {
  Init_Glfw();
  Init_ImGui();
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::Init_Glfw() {
  glfwSetErrorCallback([](int error, const char* description) { LOG_ERROR("GLFW Error %i: %s", error, description); });
  if (!glfwInit()) {
    LOG_ERROR("Failed to initialize GLFW");
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  m_glsl_version = "#version 410";

//  const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//  m_windowWidth = videoMode->width / 2;
//  m_windowHeight = videoMode->height / 2;

  m_window = glfwCreateWindow(1920,1080, "OpenGL Renderer", nullptr, nullptr);

  if (!m_window) {
    LOG_ERROR("Failed to create GLFW window");
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(m_window);
  glfwSetWindowPos(m_window, 100, 100);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    LOG_ERROR("Failed to initialize Glad");
    std::exit(EXIT_FAILURE);
  }


  // glfw may not have created a window at desired size
  int w, h;
  glfwGetFramebufferSize(m_window, &w, &h);
  Renderer::SetWindowSize(w, h);


  glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
      Renderer::SetWindowSize(width, height);
  });

  Input::Initialize(m_window);
}

void Window::Init_ImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  Style();
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init(m_glsl_version);
}

void Window::DockSpace(bool imguiFullScreen) {
  // from: https://gist.github.com/PossiblyAShrub/0aea9511b84c34e191eaa90dd7225969
  // and https://github.com/MorcilloSanz/RendererGL/tree/main

  // I didn't feel like learning this at the time I'm writing this, (want to focus on rendering), will come back to it

  static bool usePadding = false;
  static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
  // because it would be confusing to have two docking targets within each others.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (imguiFullScreen) {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |=
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  } else dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;


  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
  // and handle the pass-thru hole, so we ask Begin() to not render a background.
  if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
  // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
  // all active windows docked into it will lose their parent and become undocked.
  // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
  // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.


  if (imguiFullScreen) {
    if (!usePadding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    bool p_open = true;
    ImGui::Begin("DockSpace", &p_open, window_flags);
    if (!usePadding) ImGui::PopStyleVar();

    if (imguiFullScreen) ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
    }

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Options")) {
        if (ImGui::MenuItem("Close", nullptr, false, p_open != NULL)) exit(0);
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    ImGui::End();
  }

}

void Window::SetVsync(bool state) {
  glfwSwapInterval(state);
}

void Window::StartFrame() const {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Window::EndFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGui::EndFrame();
  ImGui::UpdatePlatformWindows();

  glfwSwapBuffers(m_window);
}

bool Window::ShouldClose() {
  return glfwWindowShouldClose(m_window);
}

void Window::Close() {
  glfwSetWindowShouldClose(m_window, true);
}


