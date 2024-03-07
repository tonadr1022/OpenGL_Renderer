//
// Created by Tony Adriansen on 3/5/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERA_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERA_HPP_

#include <glm/glm.hpp>

class Camera {
 public:
  virtual ~Camera() = default;
  virtual void Update(double dt) = 0;
  virtual void SetPosition(const glm::vec3& newPos) = 0;
  [[nodiscard]] virtual const glm::vec3& GetPosition() const = 0;
  [[nodiscard]] virtual const glm::mat4& GetProjectionMatrix() const = 0;
  [[nodiscard]] virtual const glm::mat4& GetViewMatrix() const = 0;
  [[nodiscard]] virtual const glm::mat4& GetVPMatrix() const = 0;
  virtual void SetAspectRatio(float aspectRatio) = 0;

};

#endif //OPENGL_RENDERER_SRC_RENDERER_CAMERA_CAMERA_HPP_
