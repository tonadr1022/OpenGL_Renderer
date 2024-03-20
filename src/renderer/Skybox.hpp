//
// Created by Tony Adriansen on 3/18/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_SKYBOX_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_SKYBOX_HPP_

#include "src/renderer/gl/VertexArray.hpp"
#include "src/renderer/gl/VertexBuffer.hpp"

class Skybox {
 public:
  Skybox();
  ~Skybox();
  void Draw();

 private:
  VertexArray m_vao;
  VertexBuffer m_vbo;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_SKYBOX_HPP_
