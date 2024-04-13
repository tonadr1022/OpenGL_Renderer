#pragma once

#define GL_SILENCE_DEPRECATION 1

#include <glm/glm.hpp>
#include <src/Common.hpp>

constexpr int ButtonCount = GLFW_KEY_LAST;
constexpr int MouseButtonStates = GLFW_MOUSE_BUTTON_LAST;

using Key = int;
using MouseButton = int;

class Input {
 public:
  enum KeyState : uint8_t {
    Down = 0b00001,
    Pressed = 0b00011,
    Up = 0b00100,
    Released = 0b01100,
    Repeat = 0b10001
  };

  static void Initialize(GLFWwindow* window);
  static void Update();
  static bool IsKeyDown(Key key);
  static bool IsKeyUp(Key key);
  static bool IsKeyPressed(Key key);
  static bool IsKeyReleased(Key key);
  static bool IsMouseDown(MouseButton key);
  static bool IsMouseUp(MouseButton key);
  static bool IsMousePressed(MouseButton key);
  static bool IsMouseReleased(MouseButton key);
  static void SetCursorPos(float x, float y);
  static void CenterCursor();
  static const glm::vec2& GetMousePosOffset();
  static const glm::vec2& GetMousePosition();
  static bool MouseMoved();

  static void init_glfw_input_callbacks(GLFWwindow* window);

  static void SetCursorVisible(bool state);
  static bool GetCursorVisible();

  static inline GLFWwindow* m_window;

 private:
  static inline glm::vec2 m_mouse_screen_pos;
  static inline glm::vec2 m_mouse_screen_offset;
  static inline glm::vec2 m_prev_mouse_screen_pos;
  static inline glm::vec2 m_mouse_scroll_offset;
  static inline bool m_mouse_moved;

  static void keypress_cb(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouse_pos_cb(GLFWwindow* window, double xpos, double ypos);
  static void mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset);
  static void mouse_button_cb(GLFWwindow* window, int button, int action, int mods);

  static inline KeyState m_key_states[ButtonCount] = {static_cast<KeyState>(0)};
  static inline KeyState m_mouse_button_states[MouseButtonStates] = {static_cast<KeyState>(0)};
};
