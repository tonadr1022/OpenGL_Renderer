//
// Created by Tony Adriansen on 3/20/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_FBOCONTAINER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_FBOCONTAINER_HPP_

#include "src/gl/FrameBuffer.hpp"
#include "src/gl/RenderBuffer.hpp"
#include "src/gl/Texture.hpp"

class FBOContainer {
 public:
  FBOContainer();
  void AttachColorBuffer(GLenum attachment, GLenum textureTarget,
                         std::unique_ptr<Texture> texture);
  void AttachRenderBuffer(std::unique_ptr<RenderBuffer> rbo);
  void ResizeTextures(uint32_t width, uint32_t height);

  // Unsafe but I trust myself :D
  [[nodiscard]] inline FrameBuffer& FBO() const { return *m_fbo; }
  [[nodiscard]] inline RenderBuffer& RBO() const { return *m_rbo; }
  [[nodiscard]] inline const std::vector<std::unique_ptr<Texture>>& Textures()
      const {
    return m_textures;
  }

 private:
  std::unique_ptr<FrameBuffer> m_fbo;
  std::unique_ptr<RenderBuffer> m_rbo;
  std::vector<std::unique_ptr<Texture>> m_textures;
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_GL_FBOCONTAINER_HPP_
