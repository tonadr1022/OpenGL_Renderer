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
  enum class Type { None, Diffuse, Specular, Emission, Normal };

  Texture(const std::string& texturePath, Type type, bool flip = true, bool mipmap = true);
  Texture(uint32_t width, uint32_t height);
  ~Texture();
  void Bind() const;
  void Bind(int slot) const;
  static void Unbind();
  [[nodiscard]] GLuint Id() const { return m_id; }
  void Resize(uint32_t width, uint32_t height);
  void SetWrapMode(GLint s, GLint t, GLint r);
  void SetWrapMode(GLint s, GLint t);
  void SetWrapMode(GLint all);

  void SetFilterMode(GLint minFilter, GLint magFilter);
  [[nodiscard]] inline Type GetType() const { return m_type; }
 private:
  GLuint m_id{0};
  Type m_type;
  void GenerateTextureFromFile(const std::string& texturePath, bool flip, bool mipmap);
  void GenerateTextureFromBuffer(unsigned char* buffer, bool mipmap, uint32_t numChannels, uint32_t width, uint32_t height);
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_TEXTURE_HPP_
