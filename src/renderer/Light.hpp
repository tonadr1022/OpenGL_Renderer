//
// Created by Tony Adriansen on 3/9/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_LIGHT_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_LIGHT_HPP_

#include "glm/glm.hpp"
#include <string>

struct Light {
  enum class Type {
    Point, Directional, Spotlight
  };

  static constexpr const char* TypeToString(Type type) {
    switch (type) {
      case Type::Point: return "Point";
      case Type::Directional: return "Directional";
      case Type::Spotlight: return "Spotlight";
    }
  }

  glm::vec3 position{0}, color{0};
  glm::vec3 specular{0.5}, ambient{0.5}, diffuse{0.5};
  float intensity{1};
  Type type;

  Light( const glm::vec3& position,  const glm::vec3& color, Type type);

};

#endif //OPENGL_RENDERER_SRC_RENDERER_LIGHT_HPP_
