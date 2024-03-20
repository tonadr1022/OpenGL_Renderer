//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_TEXTURE_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_TEXTURE_HPP_

#include "src/Common.hpp"
#include <string>
#include "src/core/HashedString.hpp"

class Texture {
 public:
  enum class SamplerType : GLenum {
    TwoD = GL_TEXTURE_2D,
    Array2D = GL_TEXTURE_2D_ARRAY,
    ThreeD = GL_TEXTURE_3D,
    Cube = GL_TEXTURE_CUBE_MAP
  };

  Texture(const std::string& texturePath, SamplerType type, bool flip = true, bool mipmap = true);
  explicit Texture(const std::vector<std::string>& texturePaths);
  Texture(uint32_t width, uint32_t height);
  ~Texture();
  void Bind() const;
  void Bind(int slot) const;
  void Unbind();
  [[nodiscard]] GLuint Id() const { return m_id; }
  void Resize(uint32_t width, uint32_t height);
  void SetWrapMode(GLint s, GLint t, GLint r);
  void SetWrapMode(GLint s, GLint t);
  void SetFilterMode(GLint minFilter, GLint magFilter);
 private:
  GLuint m_id{0};
  SamplerType m_samplerType;
  void GenerateTextureFromFile(const std::string& texturePath, bool flip, bool mipmap);
  void GenerateTextureFromBuffer(unsigned char* buffer,
                                 bool mipmap,
                                 uint32_t numChannels,
                                 uint32_t width,
                                 uint32_t height);
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_TEXTURE_HPP_
