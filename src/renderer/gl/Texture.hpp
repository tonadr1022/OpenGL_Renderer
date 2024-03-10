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

  Texture(const std::string& texturePath, Type type);
  Texture(uint32_t width, uint32_t height, bool mipmap);
  ~Texture();
  void Bind() const;
  void Bind(int slot) const;
  static void Unbind();
  [[nodiscard]] GLuint Id() const { return m_id; }
//  [[nodiscard]] GLint Slot() const { return m_slot; }
  void Resize(uint32_t width, uint32_t height);
  [[nodiscard]] uint32_t GetWidth() const;
  [[nodiscard]] uint32_t GetHeight() const;
  void SetWrapMode(GLint s, GLint t, GLint r);
  void SetWrapMode(GLint s, GLint t);
  void SetWrapMode(GLint all);

  void SetFilterMode(GLint minFilter, GLint magFilter);
  inline Type GetType() const { return m_type; }
 private:
  static int count;
  bool m_flipVertically;
  bool m_generateMipmaps;
  uint32_t m_width{0}, m_height{0}, m_numChannels{0};
  GLuint m_id{0};
  Type m_type;
//  GLint m_slot{0};
  void GenerateTextureFromFile(const std::string& texturePath);
  void GenerateTextureFromBuffer(unsigned char* buffer);
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GL_TEXTURE_HPP_
