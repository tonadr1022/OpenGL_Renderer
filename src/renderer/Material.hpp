//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_MATERIAL_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_MATERIAL_HPP_

#include <glm/glm.hpp>
#include <vector>

#include "src/gl/Shader.hpp"
#include "src/gl/Texture.hpp"
#include "src/utils/HashedString.hpp"

// using namespace GL;
// class Material {
//  public:
//
//   Material() = default;
//   ~Material() = default;
//   void Bind();
//   void AttachTexture(const std::shared_ptr<Texture>& texture);
//
//  private:
//   std::vector<Texture*> m_textures;
// };

using MaterialID = uint32_t;

struct PerMaterialUniformData {
  HashedString id;
  std::function<void(HashedString id, Shader& shader)> SetterFunction;
};
enum class MatTextureType { None, Diffuse, Specular, Emission, Normal };

using TexturePair = std::pair<MatTextureType, Texture*>;

struct Material {
  HashedString name;
  enum class Type { Default, BlinnPhong, Instanced, Custom };
  std::vector<TexturePair> textures;
  std::vector<PerMaterialUniformData> materialUniforms;
  glm::vec3 specularColor = {1.0, 1.0, 1.0};
  glm::vec3 diffuseColor = {1.0, 1.0, 1.0};
  glm::vec3 ambientColor = {1.0, 1.0, 1.0};
  Type type;
  HashedString shaderName;
  float shininess = 32;
  float strength = 1;
  [[nodiscard]] Shader* GetShader() const;

  Material(Material& other) = delete;

  Material(HashedString name, Material& other)
      : name(name),
        textures(other.textures),
        materialUniforms(other.materialUniforms),
        specularColor(other.specularColor),
        diffuseColor(other.diffuseColor),
        ambientColor(other.ambientColor),
        type(other.type),
        shaderName(other.shaderName),
        shininess(other.shininess),
        strength(other.strength) {}

  Material(HashedString name, const std::vector<TexturePair>& textures,
           const std::vector<PerMaterialUniformData>& materialUniforms, Type type,
           HashedString shaderName = "")
      : name(name),
        textures(textures),
        materialUniforms(materialUniforms),
        type(type),
        shaderName(shaderName) {}

  Material(HashedString name, const std::vector<TexturePair>& textures, Type type)
      : name(name), textures(textures), type(type), shaderName("") {}

  Material(HashedString name, const glm::vec3& color, Type type)
      : name(name), type(type), shaderName("") {}

  Material(HashedString name, HashedString shaderName, Type type)
      : name(name), type(type), shaderName(shaderName) {}
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_MATERIAL_HPP_
