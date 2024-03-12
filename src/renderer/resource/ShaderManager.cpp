//
// Created by Tony Adriansen on 3/2/24.
//

#include "ShaderManager.hpp"
#include <unordered_map>
#include "src/renderer/gl/Shader.hpp"
#include "src/core/Logger.hpp"
#include "src/core/Utils.hpp"

std::unordered_map<HashedString, std::unique_ptr<Shader>> ShaderManager::m_shaders;
std::unordered_map<uint32_t, ShaderManager::ShaderData> ShaderManager::m_shaderData;

Shader* ShaderManager::AddShader(HashedString name, const std::vector<ShaderCreateInfo>& createInfos) {
  if (m_shaders.contains(name)) {
    LOG_ERROR("Duplicate shader name: %s", name.data());
    return nullptr;
  }
  std::optional<ShaderData> data = CompileProgram(name, createInfos);
  if (!data.has_value()) return nullptr; // compilation failed

  auto it = m_shaderData.emplace(name, std::move(data.value()));
  // need to access from it since data got moved
  m_shaders.emplace(name, std::make_unique<Shader>(it.first->second.id, it.first->second.uniformIds));
  return m_shaders.at(name).get();
}

Shader* ShaderManager::RecompileShader(HashedString name) {
  if (!m_shaders.contains(name)) {
    LOG_ERROR("Shader not found, cannot recompile: %s", name.data());
    return nullptr;
  }
  auto oldIt = m_shaderData.find(name);

  std::optional<ShaderData> data = CompileProgram(name, oldIt->second.moduleCreateInfos);
  if (!data.has_value()) {
    LOG_ERROR("Recompilation Failed for shader: %s", name.data());
    return nullptr;
  }

  // compilation succeeded, need to delete old program, erase from map, and add new program
  glDeleteProgram(oldIt->second.id);
  m_shaderData.erase(oldIt);
  auto it = m_shaderData.emplace(name, std::move(data.value()));
  m_shaders[name] = std::make_unique<Shader>(it.first->second.id, it.first->second.uniformIds);
  return m_shaders.at(name).get();
}

Shader* ShaderManager::GetShader(HashedString name) {
  auto it = m_shaders.find(name);
  if (it == m_shaders.end()) {
    LOG_ERROR("Shader not found: %s", name.data());
    return nullptr;
  }
  return it->second.get();
}

bool ShaderManager::CheckModuleCompilationSuccess(GLuint id, std::string_view shaderPath, ShaderType type) {
  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char buffer[512];
    glGetShaderInfoLog(id, 512, nullptr, buffer);
    LOG_ERROR("File: %s", shaderPath);

    LOG_ERROR("Error Compiling Shader: %s, %s", type == ShaderType::VERTEX
                                            ? "Vertex" : type == ShaderType::FRAGMENT
                                                         ? "Fragment" : "Geometry", buffer);
    return false;
  }
  return true;
}

bool CheckProgramLinkSuccess(GLuint id) {
  glLinkProgram(id);
  int success;
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    char buffer[512];
    glGetProgramInfoLog(id, 512, nullptr, buffer);
    LOG_ERROR("Shader Link");
    return false;
  }
  return true;
}

uint32_t ShaderManager::CompileModule(ShaderType type, const std::string& src, const std::string& path) {
  GLuint shaderModule = glCreateShader((GLenum) type);
  const char* srcC = src.c_str();
  glShaderSource(shaderModule, 1, &srcC, nullptr);
  glCompileShader(shaderModule);
  CheckModuleCompilationSuccess(shaderModule, path, type);
  return shaderModule;
}

void ShaderManager::InitializeUniforms(ShaderData& shaderData) {
  // https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
  GLint uniformCount = 0; // number of uniforms
  glGetProgramiv(shaderData.id, GL_ACTIVE_UNIFORMS, &uniformCount);
  if (uniformCount > 0) {
    GLint uniformSize;
    GLenum uniformType = GL_NONE;

    GLint maxNameLength = 0;
    glGetProgramiv(shaderData.id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

    GLchar uniformName[maxNameLength + 10];
    GLsizei uniformNameLength;

    for (GLint i = 0; i < uniformCount; i++) {
      glGetActiveUniform(shaderData.id, i, maxNameLength, &uniformNameLength,
                         &uniformSize, &uniformType, uniformName);
      shaderData.uniformIds.emplace(HashedString(uniformName), glGetUniformLocation(shaderData.id, uniformName));
    }
  }

}

std::optional<ShaderManager::ShaderData> ShaderManager::CompileProgram(HashedString name,
                                                                       const std::vector<ShaderCreateInfo>& moduleCreateInfos) {
  std::vector<GLuint> moduleIds;
  for (auto& [path, type] : moduleCreateInfos) {
    std::string src = Utils::LoadFromFile(path);
    GLuint shaderId = CompileModule(type, src, path);
    moduleIds.push_back(shaderId);
  }


  GLuint programId = glCreateProgram();
  for (auto& moduleId : moduleIds) {
    glAttachShader(programId, moduleId);
  }

  glLinkProgram(programId);
  if (!CheckProgramLinkSuccess(programId)) {
    return std::nullopt;
  }

  for (auto& module : moduleIds) {
    glDetachShader(programId, module);
  }

  for (auto& module : moduleIds) {
    glDeleteShader(module);
  }
  ShaderData shaderData;
  shaderData.moduleCreateInfos = moduleCreateInfos;
  shaderData.id = programId;
  shaderData.name = name.data();
  InitializeUniforms(shaderData);

  return shaderData;
}


void ShaderManager::RecompileShaders() {
  for (auto& shader: m_shaders) {
    RecompileShader(shader.first);
  }
  LOG_INFO("Shaders Recompiled");
}