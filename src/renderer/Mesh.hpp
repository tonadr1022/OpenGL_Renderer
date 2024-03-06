//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_MESH_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_MESH_HPP_

#include <glm/glm.hpp>
#include "src/renderer/gl/VertexArray.hpp"
#include "src/renderer/gl/VertexBuffer.hpp"
#include "src/renderer/gl/ElementBuffer.hpp"
#include "src/renderer/gl/Texture.hpp"

#include <vector>
#include <memory>

#include <vector>
struct Vertex {
  glm::vec3 position;
  glm::vec2 texCoords;
};


class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
  ~Mesh();
  void SetWireFrameMode(bool state);
  [[nodiscard]] inline uint32_t NumVertices() const {return m_verticesLength; }
  [[nodiscard]] inline uint32_t NumIndices() const {return m_indicesLength; }
  inline const VertexArray& GetVAO() const {return m_vao; }

 private:
  uint32_t m_verticesLength{0}, m_indicesLength{0};
  VertexArray m_vao;
  VertexBuffer m_vbo;
  ElementBuffer m_ebo;
};


#endif //OPENGL_RENDERER_SRC_RENDERER_MESH_HPP_
