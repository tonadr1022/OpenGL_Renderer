#include "src/renderer/Material.hpp"

#include "src/resource/ShaderManager.hpp"

Shader* Material::GetShader() const {
  switch (type) {
    case Type::Default:
    case Type::BlinnPhong:
      return ShaderManager::GetShader("blinnPhong");
    case Type::Instanced:
      return ShaderManager::GetShader("instancedDefault");
    case Type::Custom:
      return ShaderManager::GetShader(shaderName);
  }
}
