//
// Created by Tony Adriansen on 3/4/24.
//

#include "TextureManager.hpp"

std::unordered_map<HashedString, std::unique_ptr<Texture>> TextureManager::m_textures;

Texture* TextureManager::GetTexture(HashedString name) {
  auto it = m_textures.find(name);
  if (it == m_textures.end()) {
    //    LOG_ERROR("Texture not found: %s", name.data());
    return nullptr;
  }
  return it->second.get();
}
