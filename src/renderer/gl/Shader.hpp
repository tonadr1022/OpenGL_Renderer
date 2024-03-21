//
// Created by Tony Adriansen on 2/29/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_SHADER_SHADER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_SHADER_SHADER_HPP_

#include "src/Common.hpp"

#include <string>
#include "glm/glm.hpp"
#include "src/core/HashedString.hpp"



class Shader {
 public:
  void Bind() const;
  static void Unbind();

  void SetInt(HashedString name, int value) const;
  void SetFloat(HashedString name, float value) const;
  void SetMat4(HashedString name, const glm::mat4& mat) const;
  void SetIVec2(HashedString name, const glm::ivec2& vec) const;
  void SetIVec3(HashedString name, const glm::ivec3& vec) const;
  void SetVec3(HashedString name, const glm::vec3& vec) const;
  void SetVec4(HashedString name, const glm::vec4& vec) const;
  void SetMat3(HashedString name, const glm::mat3& vec, bool transpose = false) const;
  void SetBool(HashedString name, bool value)const;
  Shader(uint32_t id, std::unordered_map<uint32_t, uint32_t>& uniformLocations);
  ~Shader();
  inline uint32_t Id() const {return m_id; }
 private:
  uint32_t m_id{0};
  std::unordered_map<uint32_t, uint32_t>& m_uniformLocations;

};


#endif //OPENGL_RENDERER_SRC_RENDERER_GL_SHADER_SHADER_HPP_
