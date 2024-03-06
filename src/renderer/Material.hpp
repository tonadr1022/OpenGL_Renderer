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
  std::vector<Texture*> textures;
  std::vector<PerMaterialUniformData> materialUniforms;
  Shader* shader;

  Material(const std::vector<Texture*>& textures,
           const std::vector<PerMaterialUniformData>& materialUniforms,
           HashedString shaderName)
      : textures(textures), materialUniforms(materialUniforms), shader(ShaderManager::GetShader(shaderName)) {
  }
};

#endif //OPENGL_RENDERER_SRC_RENDERER_MATERIAL_HPP_
