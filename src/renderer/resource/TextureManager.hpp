//
// Created by Tony Adriansen on 3/4/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_RESOURCE_TEXTUREMANAGER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_RESOURCE_TEXTUREMANAGER_HPP_

#include "src/renderer/gl/Texture.hpp"


class TextureManager {
 public:
  template<typename... Args>
  static Texture* AddTexture(HashedString name, Args&&... args) {
    m_textures[name] = std::make_unique<Texture>(std::forward<Args>(args)...);
    return m_textures.at(name).get();
  }

  static Texture* GetTexture(HashedString name);
 private:
  static std::unordered_map<HashedString , std::unique_ptr<Texture>> m_textures;


};

#endif //OPENGL_RENDERER_SRC_RENDERER_RESOURCE_TEXTUREMANAGER_HPP_
