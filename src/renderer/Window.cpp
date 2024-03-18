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
#include "src/core/Input.hpp"
#include "Application.hpp"

#define NUM_SAMPLES 4

Window::Window() {
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
  glfwWindowHint(GLFW_SAMPLES, NUM_SAMPLES);

  m_glsl_version = "#version 410";

//  const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//  m_windowWidth = videoMode->width / 2;
//  m_windowHeight = videoMode->height / 2;

  m_window = glfwCreateWindow(1600,900, "OpenGL Renderer", nullptr, nullptr);

  if (!m_window) {
    LOG_ERROR("Failed to create GLFW window");
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(m_window);
//  glfwSetWindowPos(m_window, 100, 100);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    LOG_ERROR("Failed to initialize Glad");
    std::exit(EXIT_FAILURE);
  }


  // glfw may not have created a window at desired size
  int w, h;
  glfwGetFramebufferSize(m_window, &w, &h);
  m_framebufferWidth = w;
  m_framebufferHeight = h;

  glfwGetWindowSize(m_window, &w, &h);
  m_windowWidth = w;
  m_windowHeight = h;

  glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
    Application::Instance().OnViewportResize(width, height);
  });
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

void Window::SetVsync(bool state) {
  glfwSwapInterval(state);
}

bool Window::ShouldClose() {
  return glfwWindowShouldClose(m_window);
}

void Window::Close() {
  glfwSetWindowShouldClose(m_window, true);
}

float Window::GetAspectRatio() const {
  return static_cast<float>(m_framebufferWidth) / static_cast<float>(m_framebufferHeight);
}

void Window::SwapBuffers() {
  glfwSwapBuffers(m_window);
}

glm::ivec2 Window::GetWindowDimensions() const {
  return {m_windowWidth, m_windowHeight};
}

glm::ivec2 Window::GetFrameBufferDimensions() const {
  return {m_framebufferWidth, m_framebufferHeight};
}
void Window::SetShouldClose(bool shouldClose) {
  glfwSetWindowShouldClose(m_window, shouldClose);
}


