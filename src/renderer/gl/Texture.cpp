//
// Created by Tony Adriansen on 3/1/24.
//

#include "Texture.hpp"

#include "stb_image/stb_image_impl.hpp"
#include "src/core/Logger.hpp"

Texture::Texture(const std::string& texturePath, Type type, bool flip, bool mipmap)
    : m_type(type) {
  GenerateTextureFromFile(texturePath, flip, mipmap);
}

Texture::Texture(uint32_t width, uint32_t height) : m_type(Type::None) {
  GenerateTextureFromBuffer(nullptr, false, 3, width, height);
}

Texture::~Texture() {
  glDeleteTextures(1, &m_id);
}

void Texture::Resize(uint32_t width, uint32_t height) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

//uint32_t Texture::GetWidth() const {
//  return m_width;
//}
//
//uint32_t Texture::GetHeight() const {
//  return m_height;
//}

void Texture::GenerateTextureFromBuffer(unsigned char* buffer,
                                        bool mipmap,
                                        uint32_t numChannels,
                                        uint32_t width,
                                        uint32_t height) {
  glGenTextures(1, &m_id);
  Bind();
  GLenum format;
  switch (numChannels) {
    case 1:format = GL_RED;
      break;
    case 3:format = GL_RGB;
      break;
    case 4:format = GL_RGBA;
      break;
    default:format = GL_RGB;
      break;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, format, (GLsizei) width, (GLsizei) height, 0, format, GL_UNSIGNED_BYTE, buffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);
//  m_slot = GL_TEXTURE0 + count;
//  count++;
}

void Texture::Bind() const {
  glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Bind(int slot) const {
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::GenerateTextureFromFile(const std::string& texturePath, bool flip, bool mipmap) {
  if (flip) {
    stbi_set_flip_vertically_on_load(true);
  }
  int numChannels, width, height;
  unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numChannels, 0);
  if (data) {
    GenerateTextureFromBuffer(data, mipmap, numChannels, width, height);
    stbi_image_free(data);
  } else {
    LOG_ERROR("Error loading Texture: %s", texturePath);
  }
}

void Texture::SetWrapMode(GLint s, GLint t, GLint r) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, r);
}

void Texture::SetWrapMode(GLint s, GLint t) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void Texture::SetWrapMode(GLint all) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, all);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, all);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, all);
}

void Texture::SetFilterMode(GLint minFilter, GLint magFilter) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}
