//
// Created by Tony Adriansen on 2/29/2024.
//

#include "Shader.hpp"
#include "src/core/Logger.hpp"

#include <sstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>

void Shader::Bind() const {
  ASSERT(m_id != 0, "Shader is valid");
  glUseProgram(m_id);
}

void Shader::Unbind() {
  glUseProgram(0);
}

void Shader::SetInt(HashedString name, int value) const {
  ASSERT(m_uniformLocations.contains(name), "Uniform name not found");
  glUniform1i(m_uniformLocations[name], value);
}

void Shader::SetFloat(HashedString name, float value) const {
  ASSERT(m_uniformLocations.contains(name), "Uniform name not found");
  glUniform1f(m_uniformLocations[name], value);

}

void Shader::SetMat4(HashedString name, const glm::mat4& mat) const {
  ASSERT(m_uniformLocations.contains(name), "Uniform name not found");
  glUniformMatrix4fv(m_uniformLocations[name], 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetIVec2(HashedString name, const glm::ivec2& vec) const {
  ASSERT(m_uniformLocations.contains(name), "Uniform name not found");
  glUniform2i(m_uniformLocations[name], vec[0], vec[1]);
}

void Shader::SetIVec3(HashedString name, const glm::ivec3& vec) const {
  ASSERT(m_uniformLocations.contains(name), "Uniform name not found");
  glUniform3iv(m_uniformLocations[name], 1, glm::value_ptr(vec));
}

void Shader::SetVec3(HashedString name, const glm::vec3& vec) const {
  ASSERT(m_uniformLocations.contains(name), "Uniform name not found");
  glUniform3fv(m_uniformLocations[name], 1, glm::value_ptr(vec));
}

void Shader::SetMat3(HashedString name, const glm::mat3& mat, bool transpose) const {
  ASSERT(m_uniformLocations.contains(name), "Uniform name not found");
  glUniformMatrix3fv(m_uniformLocations[name], 1, transpose, glm::value_ptr(mat));
}

void Shader::SetBool(HashedString name, bool value) const {
  ASSERT(m_uniformLocations.contains(name), "Uniform name not found");
  glUniform1i(m_uniformLocations[name], value);
}


Shader::Shader(uint32_t id, std::unordered_map<uint32_t, uint32_t>& uniformLocations)
    : m_id(id), m_uniformLocations(uniformLocations) {}


