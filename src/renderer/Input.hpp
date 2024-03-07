#pragma once


#define GL_SILENCE_DEPRECATION 1
#include "src/Common.hpp"
#include <glm/glm.hpp>


constexpr int BUTTON_COUNT = GLFW_KEY_LAST;
constexpr int MOUSE_BUTTON_STATES = GLFW_MOUSE_BUTTON_LAST;

using Key = int;
using MouseButton = int;

class Input {
 public:
  enum KeyState : uint8_t
  {
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

  static void init_glfw_input_callbacks(GLFWwindow* window);

  static void SetCursorVisible(bool state);
  static bool GetCursorVisible();

  static inline GLFWwindow* m_window;
 private:

  static inline glm::vec2 screenPos;
  static inline glm::vec2 screenOffset;
  static inline glm::vec2 prevScreenPos;
  static inline glm::vec2 scrollOffset;

  static void keypress_cb(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouse_pos_cb(GLFWwindow* window, double xpos, double ypos);
  static void mouse_scroll_cb(GLFWwindow* window, double xoffset, double yoffset);
  static void mouse_button_cb(GLFWwindow* window, int button, int action, int mods);

  static inline KeyState keyStates[BUTTON_COUNT] = { KeyState(0) };
  static inline KeyState mouseButtonStates[MOUSE_BUTTON_STATES] = { KeyState(0) };
};