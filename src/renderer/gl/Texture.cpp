//
// Created by Tony Adriansen on 3/1/24.
//

#include "Texture.hpp"

#include "stb_image/stb_image_impl.hpp"
#include "src/core/Logger.hpp"

Texture::Texture(const std::string& texturePath, SamplerType type, bool flip, bool mipmap)
    : m_samplerType(type) {
  GenerateTextureFromFile(texturePath, flip, mipmap);
}

Texture::Texture(uint32_t width, uint32_t height, uint32_t numSamples) {
  if (numSamples == 1) { m_samplerType = SamplerType::TwoD; }
  else if (numSamples > 1) { m_samplerType = SamplerType::TwoDMultiSample; }
  else { LOG_ERROR("Invalid number of samples: %i", numSamples); }

  GenerateTextureFromBuffer(nullptr, false, 3, width, height, numSamples);
}

Texture::~Texture() {
  glDeleteTextures(1, &m_id);
}

void Texture::Resize(uint32_t width, uint32_t height) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Texture::GenerateTextureFromBuffer(unsigned char* buffer, bool mipmap, uint32_t numChannels,
                                        uint32_t width, uint32_t height, uint32_t numSamples) {
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
  switch (m_samplerType) {
    case SamplerType::TwoD:
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   format,
                   (GLsizei) width,
                   (GLsizei) height,
                   0,
                   format,
                   GL_UNSIGNED_BYTE,
                   buffer);
      break;
    case SamplerType::TwoDMultiSample:
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                              (GLsizei)numSamples,
                              GL_RGB,
                              (GLsizei) width,
                              (GLsizei) height,
                              GL_TRUE);
      break;
  }

  glTexParameteri((GLenum) m_samplerType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri((GLenum) m_samplerType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri((GLenum) m_samplerType, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri((GLenum) m_samplerType, GL_TEXTURE_WRAP_T, GL_REPEAT);

  if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind() const {
  GL_LOG_ERROR();
  glBindTexture((GLenum) m_samplerType, m_id);
  GL_LOG_ERROR();
}
void Texture::Bind(int slot) const {
//  std::cout << slot-GL_TEXTURE0 << '\n';
  GL_LOG_ERROR();
  glActiveTexture(slot);
  glBindTexture(static_cast<GLenum>(m_samplerType), m_id);
  GL_LOG_ERROR();
}

void Texture::Unbind() {
  GL_LOG_ERROR();
  glBindTexture(static_cast<GLenum>(m_samplerType), 0);
  GL_LOG_ERROR();
}

void Texture::GenerateTextureFromFile(const std::string& texturePath, bool flip, bool mipmap) {
  if (flip) {
    stbi_set_flip_vertically_on_load(true);
  }
  int numChannels, width, height;
  unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numChannels, 0);
  if (data) {
    GenerateTextureFromBuffer(data, mipmap, numChannels, width, height, 1);
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

Texture::Texture(const std::vector<std::string>& texturePaths) :
    m_samplerType(SamplerType::Cube) {
  ASSERT(texturePaths.size() == 6, "Need 6 texture paths for cube map")
  glGenTextures(1, &m_id);
  Bind();
  GL_LOG_ERROR();
  stbi_set_flip_vertically_on_load(false);
  unsigned char* data;
  int width, height, nrChannels;
  for (uint32_t i = 0; i < texturePaths.size(); i++) {
    data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
      LOG_ERROR("Failed to load image: %s", texturePaths[i].c_str());
      stbi_image_free(data);
      continue;
    } else {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}
