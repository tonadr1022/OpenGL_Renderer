//
// Created by Tony Adriansen on 3/17/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_QUAD_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_QUAD_HPP_

#include "src/renderer/gl/VertexArray.hpp"
#include "src/renderer/gl/VertexBuffer.hpp"

class Quad {
 public:
  Quad();
  ~Quad();
  void Draw();
 private:
  VertexArray m_vao;
  VertexBuffer m_vbo;

};

#endif //OPENGL_RENDERER_SRC_RENDERER_QUAD_HPP_
