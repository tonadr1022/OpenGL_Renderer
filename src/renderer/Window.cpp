//
// Created by Tony Adriansen on 2/27/2024.
//

#include "Window.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
  glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(glfw_error_cb));
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  m_windowWidth = videoMode->width / 2;
  m_windowHeight = videoMode->height / 2;

  m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "OpenGL Renderer", nullptr, nullptr);

  if (!m_window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(m_window);
  glfwSetWindowPos(m_window, 100, 100);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    std::exit(EXIT_FAILURE);
  }


  // glfw may not have created a window at desired size
  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  m_windowWidth = width;
  m_windowHeight = height;
  glViewport(0, 0, m_windowWidth, m_windowHeight);

}

void Window::Init_ImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);

  ImGui_ImplOpenGL3_Init(m_glsl_version);
}

void Window::glfw_error_cb(int error) {
  std::cerr << "GLFW Error: " << error << std::endl;
}

void Window::framebuffer_size_cb(GLFWwindow* window, int width, int height) {
  auto* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (instance != nullptr) {
    instance->m_windowWidth = width;
    instance->m_windowHeight = height;
    glViewport(0, 0, width, height);
  }
}

void Window::SetVsync(bool state) {
  glfwSwapInterval(state);
}

void Window::StartFrame() const {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Window::EndFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(m_window);
}

bool Window::ShouldClose() {
  return glfwWindowShouldClose(m_window);
}

void Window::Close() {
  glfwSetWindowShouldClose(m_window, true);
}

float Window::AspectRatio() {
  return static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
}



