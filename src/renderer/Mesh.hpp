//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_MESH_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_MESH_HPP_

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

#include "src/gl/ElementBuffer.hpp"
#include "src/gl/VertexArray.hpp"
#include "src/gl/VertexBuffer.hpp"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
  Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
       const std::vector<glm::mat4>& model_matrices);
  ~Mesh();

  [[nodiscard]] inline uint32_t NumVertices() const { return m_verticesLength; }
  [[nodiscard]] inline uint32_t NumIndices() const { return m_indicesLength; }
  [[nodiscard]] inline const VertexArray& GetVAO() const { return m_vao; }

  void Draw() const;

  constexpr static int FinalVertexAttributeIndex = 2;

  void MakeInstanced(const std::vector<glm::mat4>& model_matrices);

 private:
  uint32_t m_verticesLength{0}, m_indicesLength{0};
  VertexArray m_vao;
  VertexBuffer m_vbo;
  ElementBuffer m_ebo;
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_MESH_HPP_
