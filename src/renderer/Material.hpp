//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_MATERIAL_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_MATERIAL_HPP_

#include "src/renderer/gl/Texture.hpp"
#include "src/renderer/gl/Shader.hpp"
#include "src/renderer/resource/ShaderManager.hpp"

#include <vector>
#include <memory>
#include <vector>

//using namespace GL;
//class Material {
// public:
//
//  Material() = default;
//  ~Material() = default;
//  void Bind();
//  void AttachTexture(const std::shared_ptr<Texture>& texture);
//
// private:
//  std::vector<Texture*> m_textures;
//};

using MaterialID = uint32_t;

struct PerMaterialUniformData {
  HashedString id;
  std::function<void(HashedString id, Shader& shader)> SetterFunction;
};

struct Material {
  enum class Type { Default, BlinnPhong };
  std::vector<Texture*> textures;
  std::vector<PerMaterialUniformData> materialUniforms;
  Shader* shader;
  glm::vec3 color = {0.0f, 0.0f, 0.0f};
  Type type;

  Material(const std::vector<Texture*>& textures,
           const std::vector<PerMaterialUniformData>& materialUniforms,
           HashedString shaderName, Type type)
      : textures(textures),
        materialUniforms(materialUniforms),
        shader(ShaderManager::GetShader(shaderName)),
        type(type) {
  }

  Material(const std::vector<Texture*>& textures, HashedString shaderName, Type type)
      : textures(textures), shader(ShaderManager::GetShader(shaderName)), type(type) {
  }

  Material(const glm::vec3& color, HashedString shaderName, Type type)
      : shader(ShaderManager::GetShader(shaderName)), color(color), type(type) {
  }

  Material(HashedString shaderName, Type type)
      : shader(ShaderManager::GetShader(shaderName)), color(glm::vec3(1, 1, 1)), type(type) {
  }

  Material(const std::vector<Texture*>& textures,
           const std::vector<PerMaterialUniformData>& materialUniforms,
           HashedString shaderName)
      : textures(textures),
        materialUniforms(materialUniforms),
        shader(ShaderManager::GetShader(shaderName)),
        type(Type::BlinnPhong) {
  }

  Material(const std::vector<Texture*>& textures, HashedString shaderName)
      : textures(textures), shader(ShaderManager::GetShader(shaderName)), type(Type::BlinnPhong) {
  }

  Material(const glm::vec3& color, HashedString shaderName)
      : shader(ShaderManager::GetShader(shaderName)), color(color), type(Type::BlinnPhong) {
  }

  explicit Material(HashedString shaderName)
      : shader(ShaderManager::GetShader(shaderName)), color(glm::vec3(1, 1, 1)), type(Type::BlinnPhong) {
  }

};

#endif //OPENGL_RENDERER_SRC_RENDERER_MATERIAL_HPP_
