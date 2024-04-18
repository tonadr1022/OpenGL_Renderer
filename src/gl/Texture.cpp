//
// Created by Tony Adriansen on 3/1/24.
//

#include "Texture.hpp"

#include <string>
#include <vector>

#include "src/utils/Logger.hpp"
#include "stb_image/stb_image_impl.hpp"

Texture::Texture(const std::string& texturePath, SamplerType type, bool flip, bool mipmap)
    : m_samplerType(type) {
  GenerateTextureFromFile(texturePath, flip, mipmap);
}

Texture::Texture(uint32_t width, uint32_t height, uint32_t numSamples) {
  if (numSamples == 1) {
    m_samplerType = SamplerType::TwoD;
  } else if (numSamples > 1) {
    m_samplerType = SamplerType::TwoDMultiSample;
  } else {
    LOG_ERROR("Invalid number of samples: %i", numSamples);
  }
  GenerateTextureFromBuffer(nullptr, false, 3, width, height, numSamples);
}

Texture::~Texture() { glDeleteTextures(1, &m_id); }

void Texture::Resize(uint32_t width, uint32_t height) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Texture::GenerateTextureFromBuffer(unsigned char* buffer, bool mipmap, uint32_t numChannels,
                                        uint32_t width, uint32_t height, uint32_t numSamples) {
  glGenTextures(1, &m_id);
  Bind();
  GLenum format;
  switch (numChannels) {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      format = GL_RGB;
      break;
  }
  switch (m_samplerType) {
    case SamplerType::TwoD:
      glTexImage2D(GL_TEXTURE_2D, 0, format, static_cast<GLsizei>(width),
                   static_cast<GLsizei>(height), 0, format, GL_UNSIGNED_BYTE, buffer);
      break;
    case SamplerType::TwoDMultiSample:
      glad_glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLsizei>(numSamples),
                                   GL_RGB, static_cast<GLsizei>(width),
                                   static_cast<GLsizei>(height), GL_TRUE);
      break;
    case SamplerType::Array2D:
    case SamplerType::ThreeD:
    case SamplerType::Cube:
      break;
  }
  if (m_samplerType != SamplerType::TwoDMultiSample) {
    glTexParameteri(static_cast<GLenum>(m_samplerType), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(static_cast<GLenum>(m_samplerType), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(static_cast<GLenum>(m_samplerType), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(static_cast<GLenum>(m_samplerType), GL_TEXTURE_WRAP_T, GL_REPEAT);
  }

  if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind() const { glBindTexture(static_cast<GLenum>(m_samplerType), m_id); }
void Texture::Bind(int slot) const {
  glActiveTexture(slot);
  glBindTexture(static_cast<GLenum>(m_samplerType), m_id);
}

void Texture::Unbind() { glBindTexture(static_cast<GLenum>(m_samplerType), 0); }

void Texture::GenerateTextureFromFile(const std::string& texturePath, bool flip, bool mipmap) {
  if (flip) {
    stbi_set_flip_vertically_on_load(true);
  }
  int num_channels;
  int width;
  int height;
  unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &num_channels, 0);
  if (data) {
    GenerateTextureFromBuffer(data, mipmap, num_channels, width, height, 1);
    stbi_image_free(data);
  } else {
    LOG_ERROR("Error loading Texture: %s", texturePath);
  }
}

void Texture::SetWrapMode(GLint s, GLint t, GLint r) {
  glTexParameteri(static_cast<GLint>(m_samplerType), GL_TEXTURE_WRAP_S, s);
  glTexParameteri(static_cast<GLint>(m_samplerType), GL_TEXTURE_WRAP_T, t);
  glTexParameteri(static_cast<GLint>(m_samplerType), GL_TEXTURE_WRAP_R, r);
}

void Texture::SetWrapMode(GLint s, GLint t) {
  glTexParameteri(static_cast<GLint>(m_samplerType), GL_TEXTURE_WRAP_S, s);
  glTexParameteri(static_cast<GLint>(m_samplerType), GL_TEXTURE_WRAP_T, t);
}

void Texture::SetFilterMode(GLint minFilter, GLint magFilter) {
  glTexParameteri(static_cast<GLint>(m_samplerType), GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(static_cast<GLint>(m_samplerType), GL_TEXTURE_MAG_FILTER, magFilter);
}

Texture::Texture(const std::vector<std::string>& texturePaths) : m_samplerType(SamplerType::Cube) {
  ASSERT(texturePaths.size() == 6, "Need 6 texture paths for cube map")
  glGenTextures(1, &m_id);
  Bind();
  stbi_set_flip_vertically_on_load(false);
  unsigned char* data;
  int width;
  int height;
  int num_channels;
  for (uint32_t i = 0; i < texturePaths.size(); i++) {
    data = stbi_load(texturePaths[i].c_str(), &width, &height, &num_channels, 0);
    if (!data) {
      LOG_ERROR("Failed to load image: %s", texturePaths[i].c_str());
      stbi_image_free(data);
      continue;
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::Screenshot(std::string_view filename) const {
  Bind();
  stbi_flip_vertically_on_write(true);
  int w;
  int h;
  int miplevel = 0;
  glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
  std::vector<GLubyte> pixels(w * h * 3);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
  std::string full_path = "screenshots/" + std::string(filename);
  stbi_write_png(full_path.c_str(), w, h, 3, pixels.data(), w * 3);
}
