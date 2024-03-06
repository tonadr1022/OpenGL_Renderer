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

  void SetInt(HashedString name, int value);
  void SetFloat(HashedString name, float value);
  void SetMat4(HashedString name, const glm::mat4& mat);
  void SetIVec2(HashedString name, const glm::ivec2& vec);
  void SetIVec3(HashedString name, const glm::ivec3& vec);
  void SetVec3(HashedString name, const glm::vec3& vec);

  Shader(uint32_t id, std::unordered_map<uint32_t, uint32_t>& uniformLocations);

 private:
  uint32_t m_id{0};
  std::unordered_map<uint32_t, uint32_t>& m_uniformLocations;

};


#endif //OPENGL_RENDERER_SRC_RENDERER_GL_SHADER_SHADER_HPP_
