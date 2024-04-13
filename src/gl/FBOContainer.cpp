//
// Created by Tony Adriansen on 3/20/24.
//

#include "FBOContainer.hpp"

FBOContainer::FBOContainer() { m_fbo = std::make_unique<FrameBuffer>(); }

void FBOContainer::AttachColorBuffer(GLenum attachment, GLenum textureTarget,
                                     std::unique_ptr<Texture> texture) {
  m_fbo->AttachColorBuffer(attachment, textureTarget, texture->Id());
  m_textures.push_back(std::move(texture));
}

void FBOContainer::ResizeTextures(uint32_t width, uint32_t height) {
  for (auto& texture : m_textures) {
    texture->Bind();
    texture->Resize(width, height);
  }
}

void FBOContainer::AttachRenderBuffer(std::unique_ptr<RenderBuffer> rbo) {
  m_rbo = std::move(rbo);
  m_rbo->Bind();
  m_fbo->AttachRenderBuffer(m_rbo->Id());
}
