//
// Created by Tony Adriansen on 3/9/24.
//

#include "Light.hpp"

LightBase::LightBase(const glm::vec3& color) : color(color) {}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color)
    : LightBase(color), position(position) {}

PointLight::PointLight(const glm::vec3& position) : position(position) {}

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction)
    : position(position), direction(direction) {}

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& color, const glm::vec3& direction)
    : LightBase(color), position(position), direction(direction) {}

DirectionalLight::DirectionalLight(const glm::vec3& color, const glm::vec3& direction)
    : LightBase(color), direction(direction) {}

DirectionalLight::DirectionalLight(const glm::vec3& direction) : direction(direction) {}
