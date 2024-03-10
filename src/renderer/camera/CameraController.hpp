//
// Created by Tony Adriansen on 3/8/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERACONTROLLER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERACONTROLLER_HPP_

#include "src/renderer/camera/OrbitCamera.hpp"
#include "src/renderer/camera/FPSCamera.hpp"
#include "src/renderer/Window.hpp"
#include <memory>

class CameraController {
 public:
  explicit CameraController(const Window& window);
  void SetAspectRatio(float aspectRatio);
  enum class Mode {
    FPS, ORBIT,
  };

  [[nodiscard]] Camera* GetActiveCamera() const;
//  [[nodiscard]] inline Mode GetMode() const { return m_mode; }

  void SetMode(Mode mode);
  void Update(double dt);
  void OnImGui();
  void OnMouseButtonEvent(int button, int action);
  void ProcessMouseMovement(double xOffset, double yOffset);
  void OnMouseScrollEvent(double yOffset);

  void Focus();
  void Unfocus();
  [[nodiscard]] inline bool IsFocused() const { return m_focused; }
 private:
  bool m_focused;
  Mode m_mode;
  Camera* m_activeCamera;
  std::unique_ptr<FPSCamera> m_fpsCamera;
  std::unique_ptr<OrbitCamera> m_orbitCamera;
  const Window& m_window;

  static constexpr Mode DEFAULT_MODE = Mode::FPS;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERACONTROLLER_HPP_
