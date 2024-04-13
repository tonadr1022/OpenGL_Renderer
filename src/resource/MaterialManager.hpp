//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_RESOURCE_MATERIALMANAGER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_RESOURCE_MATERIALMANAGER_HPP_

#include <memory>

#include "src/renderer/Material.hpp"

class MaterialManager {
 public:
  template <typename... Args>
  static MaterialID AddMaterial(HashedString name, Args&&... args) {
    m_materials.emplace(name, std::make_unique<Material>(std::forward<Args>(args)...));
    return name;
  }
  static Material* GetMaterial(HashedString name);

 private:
  static std::unordered_map<HashedString, std::unique_ptr<Material>> m_materials;
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_RESOURCE_MATERIALMANAGER_HPP_
