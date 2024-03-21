//
// Created by Tony Adriansen on 3/20/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSINGEFFECT_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSINGEFFECT_HPP_

#include "src/renderer/gl/FBOContainer.hpp"
#include "src/renderer/gl/Shader.hpp"

struct PPEffect {
  bool enabled = true;
  Shader* shader = nullptr;
  std::function<void()> uniformFunction = nullptr;
  std::unique_ptr<FBOContainer> fboContainer = nullptr;

  PPEffect(HashedString name, Shader* shader, std::unique_ptr<FBOContainer> fboContainer, const std::function<void()>& uniformFunction)
  : enabled(true), shader(shader), uniformFunction(uniformFunction), fboContainer(std::move(fboContainer)) {}

  PPEffect(HashedString name, Shader* shader, std::unique_ptr<FBOContainer> fboContainer)
      : enabled(true), shader(shader), fboContainer(std::move(fboContainer)) {}
};

class PostProcessingEffect {
 public:
  PostProcessingEffect(HashedString name,
                       Shader* shader,
                       std::unique_ptr<FBOContainer> FBOContainer,
                       const std::function<void()>& uniformFunction);
  PostProcessingEffect(HashedString name, Shader* shader, std::unique_ptr<FBOContainer> FBOContainer);
  void SetFBOContainer(std::unique_ptr<FBOContainer> FBOContainer);
  [[nodiscard]] inline FBOContainer* GetFBOContainer() const { return m_FBOContainer.get(); }
  void SetUniforms();
  [[nodiscard]] inline Shader* GetShader() const { return m_shader; }

 private:
  bool enabled = true;
  Shader* m_shader = nullptr;
  std::function<void()> m_uniformFunction = nullptr;
  std::unique_ptr<FBOContainer> m_FBOContainer = nullptr;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_POSTPROCESSINGEFFECT_HPP_
