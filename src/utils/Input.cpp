#include "Input.hpp"

#include "imgui/backends/imgui_impl_glfw.h"
#include "src/Application.hpp"

void Input::Update() {
  for (auto& mouse_button_state : m_mouse_button_states) {
    if (mouse_button_state & Pressed) mouse_button_state = Down;
    if (mouse_button_state & Released) mouse_button_state = Up;
  }

  for (auto& key_state : m_key_states) {
    if (key_state & Pressed) key_state = Down;
    if (key_state & Released) key_state = Up;
  }
  m_mouse_scroll_offset = glm::vec2(0);
  m_mouse_scroll_offset = glm::vec2(0);
  m_mouse_moved = false;
  glfwPollEvents();
}

bool Input::IsKeyDown(Key key) { return m_key_states[key] == Down; }

bool Input::IsKeyUp(Key key) { return m_key_states[key] == Down; }

bool Input::IsKeyPressed(Key key) { return m_key_states[key] == Pressed; }

bool Input::IsKeyReleased(Key key) { return m_key_states[key] == Released; }

bool Input::IsMouseDown(MouseButton key) { return m_mouse_button_states[key] == Down; }

bool Input::IsMouseUp(MouseButton key) { return m_mouse_button_states[key] == Up; }

bool Input::IsMousePressed(MouseButton key) { return m_mouse_button_states[key] == Pressed; }

bool Input::IsMouseReleased(MouseButton key) { return m_mouse_button_states[key] == Released; }

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

bool Input::GetCursorVisible() { return glfwGetInputMode(m_window, GLFW_CURSOR) != 0; }

void Input::keypress_cb(GLFWwindow* window, int key, int scancode, int action, int mods) {
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
  //  ImGuiIO& io = ImGui::GetIO();
  //  if (action == GLFW_PRESS) {
  //    io.KeysDown[key] = true;
  //  }
  //  if (action == GLFW_RELEASE) {
  //    io.KeysDown[key] = false;
  //  }

  //  io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] ||
  //  io.KeysDown[GLFW_KEY_RIGHT_CONTROL]; io.KeyShift =
  //  io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
  //  io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] ||
  //  io.KeysDown[GLFW_KEY_RIGHT_ALT]; io.KeySuper =
  //  io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

  if (action == GLFW_PRESS) {
    m_key_states[key] = Pressed;
  } else if (action == GLFW_RELEASE) {
    m_key_states[key] = Released;
  }
  Application::Instance().OnKeyEvent(key, action, mods);
}

void Input::mouse_pos_cb(GLFWwindow* window, double xpos, double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
  static double last_x;
  static double last_y;
  double x_offset = xpos - last_x;
  double y_offset = ypos - last_y;
  last_x = xpos;
  last_y = ypos;
  m_mouse_screen_offset = {x_offset, y_offset};
  m_mouse_screen_pos = {xpos, ypos};
  m_mouse_moved = true;

  Application::Instance().OnMousePosMove(xpos, ypos);
}

void Input::mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset) {
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
  Application::Instance().OnMouseScrollEvent(yoffset);
}

void Input::mouse_button_cb(GLFWwindow* window, int button, int action, int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
  if (action == GLFW_PRESS) {
    m_mouse_button_states[button] = Pressed;
  } else if (action == GLFW_RELEASE) {
    m_mouse_button_states[button] = Released;
  }
  Application::Instance().OnMouseButtonEvent(button, action);
}

void Input::Initialize(GLFWwindow* window) {
  m_window = window;
  init_glfw_input_callbacks(window);
}

void Input::SetCursorPos(float x, float y) { glfwSetCursorPos(m_window, x, y); }

void Input::CenterCursor() {
  int width;
  int height;
  glfwGetWindowSize(m_window, &width, &height);
  glfwSetCursorPos(m_window, static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f);
}

const glm::vec2& Input::GetMousePosOffset() { return m_mouse_screen_offset; }
const glm::vec2& Input::GetMousePosition() { return m_mouse_screen_pos; }

bool Input::MouseMoved() { return m_mouse_moved; }
