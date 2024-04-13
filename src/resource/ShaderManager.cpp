
// Created by Tony Adriansen on 3/2/24.
//

#include "ShaderManager.hpp"

#include <unordered_map>

#include "src/gl/Shader.hpp"
#include "src/utils/Logger.hpp"
#include "src/utils/Utils.hpp"

std::unordered_map<HashedString, std::unique_ptr<Shader>> ShaderManager::m_shaders;
std::unordered_map<uint32_t, ShaderManager::ShaderData> ShaderManager::m_shader_data;

Shader* ShaderManager::AddShader(HashedString name,
                                 const std::vector<ShaderCreateInfo>& createInfos) {
  if (m_shaders.contains(name)) {
    LOG_ERROR("Duplicate shader name: %s", name.data());
    return nullptr;
  }
  std::optional<ShaderData> data = CompileProgram(name, createInfos);
  if (!data.has_value()) return nullptr;  // compilation failed

  auto it = m_shader_data.emplace(name, std::move(data.value()));
  // need to access from it since data got moved
  m_shaders.emplace(name,
                    std::make_unique<Shader>(it.first->second.id, it.first->second.uniformIds));
  return m_shaders.at(name).get();
}

Shader* ShaderManager::RecompileShader(HashedString name) {
  if (!m_shaders.contains(name)) {
    LOG_ERROR("Shader not found, cannot recompile: %s", name.data());
    return nullptr;
  }
  auto old_it = m_shader_data.find(name);

  std::optional<ShaderData> data = CompileProgram(name, old_it->second.moduleCreateInfos);
  if (!data.has_value()) {
    LOG_ERROR("Recompilation Failed for shader: %s", name.data());
    return nullptr;
  }

  // compilation succeeded, need to delete old program, erase from map, and add
  // new program
  glDeleteProgram(old_it->second.id);
  m_shader_data.erase(old_it);
  auto it = m_shader_data.emplace(name, std::move(data.value()));
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

bool ShaderManager::CheckModuleCompilationSuccess(GLuint id, std::string_view shaderPath,
                                                  ShaderType type) {
  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char buffer[512];
    glGetShaderInfoLog(id, 512, nullptr, buffer);
    LOG_ERROR("File: %s", shaderPath);

    LOG_ERROR("Error Compiling Shader: %s, %s",
              type == ShaderType::Vertex     ? "Vertex"
              : type == ShaderType::Fragment ? "Fragment"
                                             : "Geometry",
              buffer);
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

uint32_t ShaderManager::CompileModule(ShaderType type, const std::string& src,
                                      const std::string& path) {
  GLuint shader_module = glCreateShader(static_cast<GLenum>(type));
  const char* src_c = src.c_str();
  glShaderSource(shader_module, 1, &src_c, nullptr);
  glCompileShader(shader_module);
  CheckModuleCompilationSuccess(shader_module, path, type);
  return shader_module;
}

void ShaderManager::InitializeUniforms(ShaderData& shaderData) {
  // https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
  GLint uniform_count = 0;  // number of uniforms
  glGetProgramiv(shaderData.id, GL_ACTIVE_UNIFORMS, &uniform_count);
  if (uniform_count > 0) {
    GLint uniform_size;
    GLenum uniform_type = GL_NONE;

    //    GLint maxNameLength = 0;
    //    glGetProgramiv(shaderData.id, GL_ACTIVE_UNIFORM_MAX_LENGTH,
    //    &maxNameLength);

    GLchar uniform_name[60];
    GLsizei uniform_name_length;

    for (GLint i = 0; i < uniform_count; i++) {
      glGetActiveUniform(shaderData.id, i, 60, &uniform_name_length, &uniform_size, &uniform_type,
                         uniform_name);
      shaderData.uniformIds.emplace(HashedString(uniform_name),
                                    glGetUniformLocation(shaderData.id, uniform_name));
    }
  }
}

std::optional<ShaderManager::ShaderData> ShaderManager::CompileProgram(
    HashedString name, const std::vector<ShaderCreateInfo>& moduleCreateInfos) {
  std::vector<GLuint> module_ids;
  for (const auto& [path, type] : moduleCreateInfos) {
    std::string src = Utils::LoadFromFile(path);
    GLuint shader_id = CompileModule(type, src, path);
    module_ids.push_back(shader_id);
  }

  GLuint program_id = glCreateProgram();
  for (auto& module_id : module_ids) {
    glAttachShader(program_id, module_id);
  }

  glLinkProgram(program_id);
  if (!CheckProgramLinkSuccess(program_id)) {
    return std::nullopt;
  }

  for (auto& module : module_ids) {
    glDetachShader(program_id, module);
  }

  for (auto& module : module_ids) {
    glDeleteShader(module);
  }
  ShaderData shader_data;
  shader_data.moduleCreateInfos = moduleCreateInfos;
  shader_data.id = program_id;
  shader_data.name = name.data();
  InitializeUniforms(shader_data);

  return shader_data;
}

void ShaderManager::RecompileShaders() {
  for (auto& shader : m_shaders) {
    RecompileShader(shader.first);
  }
  LOG_INFO("Shaders Recompiled");
}
