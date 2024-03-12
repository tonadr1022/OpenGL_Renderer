//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_RESOURCE_SHADERMANAGER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_RESOURCE_SHADERMANAGER_HPP_

#include <cstdint>

#include "src/renderer/gl/Shader.hpp"
#include "src/core/HashedString.hpp"

enum class ShaderType {
  VERTEX = GL_VERTEX_SHADER,
  GEOMETRY = GL_GEOMETRY_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER
};

struct ShaderCreateInfo {
  std::string shaderPath;
  ShaderType shaderType;
};

class ShaderManager {
 public:
  static Shader* GetShader(HashedString name);
  static Shader* AddShader(HashedString name, const std::vector<ShaderCreateInfo>& createInfos);
  static Shader* RecompileShader(HashedString name);
  static void RecompileShaders();

 private:
  struct ShaderData {
    std::string name;
    uint32_t id{0};
    std::unordered_map<uint32_t, uint32_t> uniformIds;
    std::vector<ShaderCreateInfo> moduleCreateInfos;
  };

  static bool CheckModuleCompilationSuccess(GLuint id, std::string_view shaderPath, ShaderType type);
  static uint32_t CompileModule(ShaderType type, const std::string& src, const std::string& path);
  static void InitializeUniforms(ShaderData& shaderData);
  static std::optional<ShaderData> CompileProgram(HashedString name,
                                                  const std::vector<ShaderCreateInfo>& moduleCreateInfos);

  static std::unordered_map<HashedString, std::unique_ptr<Shader>> m_shaders;
  static std::unordered_map<uint32_t, ShaderData> m_shaderData;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_RESOURCE_SHADERMANAGER_HPP_
