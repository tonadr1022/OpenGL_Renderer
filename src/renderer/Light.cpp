//
// Created by Tony Adriansen on 3/9/24.
//

#include "Light.hpp"

Light::Light(const  glm::vec3& position, const glm::vec3& color, Type type)
    : position(position), color(color), type(type) {
}