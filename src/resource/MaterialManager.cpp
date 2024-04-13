//
// Created by Tony Adriansen on 3/2/24.
//

#include "MaterialManager.hpp"
#include "src/utils/Logger.hpp"

#include <unordered_map>

std::unordered_map<HashedString, std::unique_ptr<Material>> MaterialManager::m_materials;



Material* MaterialManager::GetMaterial(HashedString name) {
  auto it = m_materials.find(name);
  if (it == m_materials.end()) {
//    LOG_INFO("Material not found: %s", name.data());
    return nullptr;}
  return it->second.get();
}
