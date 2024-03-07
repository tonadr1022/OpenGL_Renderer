#include "Input.hpp"
#include "src/core/Logger.hpp"
#include "Application.hpp"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

void Input::Update() {
  for (auto& mouseButtonState : mouseButtonStates) {
    if (mouseButtonState & Pressed) mouseButtonState = Down;
    if (mouseButtonState & Released) mouseButtonState = Up;
  }

  for (auto& keyState : keyStates) {
    if (keyState & Pressed) keyState = Down;
    if (keyState & Released) keyState = Up;
  }
  scrollOffset = glm::vec2(0);
  screenOffset = glm::vec2(0);
  glfwPollEvents();
}

bool Input::IsKeyDown(Key key) {
  return keyStates[key] == Down;
}

bool Input::IsKeyUp(Key key) {
  return keyStates[key] == Down;
}

bool Input::IsKeyPressed(Key key) {
  return keyStates[key] == Pressed;
}

bool Input::IsKeyReleased(Key key) {
  return keyStates[key] == Released;
}

bool Input::IsMouseDown(MouseButton key) {
  return mouseButtonStates[key] == Down;
}

bool Input::IsMouseUp(MouseButton key) {
  return mouseButtonStates[key] == Up;
}

bool Input::IsMousePressed(MouseButton key) {
  return mouseButtonStates[key] == Pressed;
}

bool Input::IsMouseReleased(MouseButton key) {
  return mouseButtonStates[key] == Released;
}

void Input::init_glfw_input_callbacks(GLFWwindow* window) {
  m_window = window;

  glfwSetKeyCallback(window, keypress_cb);
  glfwSetCursorPosCallback(window, mouse_pos_cb);
  glfwSetMouseButtonCallback(window, mouse_button_cb);
  glfwSetScrollCallback(window, mouse_scroll_cb);

  glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);

}

void Input::SetCursorVisible(bool state) {
  int mode = state ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
  glfwSetInputMode(m_window, GLFW_CURSOR, mode);
}

bool Input::GetCursorVisible() {
  return glfwGetInputMode(m_window, GLFW_CURSOR);
}

void Input::keypress_cb(GLFWwindow* window, int key, int scancode, int action, int mods) {
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
  ImGuiIO& io = ImGui::GetIO();
  if (action == GLFW_PRESS) {
    io.KeysDown[key] = true;
  }
  if (action == GLFW_RELEASE) {
    io.KeysDown[key] = false;
  }

  io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
  io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

  if (action == GLFW_PRESS) {
    keyStates[key] = Pressed;
  }
  if (action == GLFW_RELEASE) {
    keyStates[key] = Released;
  }
//  Application::Instance().OnKeyEvent(key, action);
}

void Input::mouse_pos_cb(GLFWwindow* window, double xpos, double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
  Application::Instance().OnMousePosMove(xpos, ypos);
}

void Input::mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset) {
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void Input::mouse_button_cb(GLFWwindow* window, int button, int action, int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
  Application::Instance().OnMouseButtonEvent(button, action);
}

void Input::Initialize(GLFWwindow* window) {
  m_window = window;
  init_glfw_input_callbacks(window);
}

