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
enum class MatTextureType {
  None, Diffuse, Specular, Emission, Normal
};

using TexturePair = std::pair<MatTextureType, Texture*>;

struct Material {
  enum class Type { Default, BlinnPhong };
  std::vector<TexturePair> textures;
  std::vector<PerMaterialUniformData> materialUniforms;
  HashedString shaderName;
  glm::vec3 specularColor = {1.0,1.0,1.0};
  glm::vec3 diffuseColor = {1.0,1.0,1.0};
  glm::vec3 ambientColor = {1.0,1.0,1.0};
  Type type;
  float shininess = 32;
  float strength = 1;

  Material(const std::vector<TexturePair>& textures,
           const std::vector<PerMaterialUniformData>& materialUniforms,
           HashedString shaderName, Type type)
      : textures(textures),
        materialUniforms(materialUniforms),
        shaderName(shaderName),
        type(type) {
  }

  Material(const std::vector<TexturePair>& textures, HashedString shaderName, Type type)
      : textures(textures), shaderName(shaderName), type(type) {
  }

  Material(const glm::vec3& color, HashedString shaderName, Type type)
      : shaderName(shaderName), type(type) {
  }

  Material(HashedString shaderName, Type type)
      : shaderName(shaderName), type(type) {
  }

  Material(const std::vector<TexturePair>& textures,
           const std::vector<PerMaterialUniformData>& materialUniforms,
           HashedString shaderName)
      : textures(textures),
        materialUniforms(materialUniforms),
        shaderName(shaderName),
        type(Type::BlinnPhong) {
  }

  Material(const std::vector<TexturePair>& textures, HashedString shaderName)
      : textures(textures), shaderName(shaderName), type(Type::BlinnPhong) {
  }

  Material(const glm::vec3& color, HashedString shaderName)
      : shaderName(shaderName), type(Type::BlinnPhong) {
  }

  explicit Material(HashedString shaderName)
      : shaderName(shaderName), type(Type::BlinnPhong) {
  }

};

#endif //OPENGL_RENDERER_SRC_RENDERER_MATERIAL_HPP_
