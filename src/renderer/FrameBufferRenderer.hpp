//
// Created by Tony Adriansen on 3/5/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_FRAMEBUFFERRENDERER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_FRAMEBUFFERRENDERER_HPP_
#include <memory>
#include "src/renderer/gl/Texture.hpp"
#include "src/renderer/gl/FrameBuffer.hpp"
#include "src/renderer/gl/RenderBuffer.hpp"

class FrameBufferRenderer {
 public:
  FrameBufferRenderer(uint32_t Width, uint32_t Height);
  void UpdateViewport(uint32_t Width, uint32_t Height);
  [[nodiscard]] inline const Texture& GetTexture() const { return m_texture; };

  void StartCapture();
  void EndCapture();

 private:
  FrameBuffer m_fbo;
  RenderBuffer m_rbo;
  Texture m_texture;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_FRAMEBUFFERRENDERER_HPP_
