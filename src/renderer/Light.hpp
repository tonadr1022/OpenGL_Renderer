//
// Created by Tony Adriansen on 3/9/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_LIGHT_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_LIGHT_HPP_

#include "glm/glm.hpp"
#include <string>

#define DEFAULT_SPECULAR_STRENGTH 1.0;
#define DEFAULT_AMBIENT_STRENGTH  0.2;
#define DEFAULT_DIFFUSE_STRENGTH  0.5;

struct LightBase {
  glm::vec3 color = {1.0f,1.0f,1.0f};
  float ambientIntensity = 0.2f;
  float diffuseIntensity = 0.5f;
  float specularIntensity = 1.0f;

  explicit LightBase(const glm::vec3& color);
  LightBase() = default;
};

struct PointLight : public LightBase {
  glm::vec3 position;
//  float linear = 0.09;
//  float quadratic = 0.032;
  float radius = 10;

  // Default constructor
  PointLight(const glm::vec3& position, const glm::vec3& color);
  explicit PointLight(const glm::vec3& position);
};

struct SpotLight : public LightBase {
  glm::vec3 position;
  glm::vec3 direction;
  float radius = 10;
//  float linear = 0.09;
//  float quadratic = 0.032;
  float penumbra = 5.0f;
  float angle = 17.5f;

  SpotLight(const glm::vec3& position, const glm::vec3& color, const glm::vec3& direction);
  SpotLight(const glm::vec3& position, const glm::vec3& direction);
};

struct DirectionalLight : public LightBase {
  glm::vec3 direction;

  DirectionalLight(const glm::vec3& color, const glm::vec3& direction);
  explicit DirectionalLight(const glm::vec3& direction);
};

#endif //OPENGL_RENDERER_SRC_RENDERER_LIGHT_HPP_
