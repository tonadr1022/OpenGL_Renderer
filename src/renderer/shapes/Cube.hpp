//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_SHAPES_CUBE_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_SHAPES_CUBE_HPP_

#include "src/renderer/Mesh.hpp"
#include <memory>

namespace Cube {

const std::vector<uint32_t> Indices =
    {
        0, 1, 3, 1, 2, 3,
        4, 7, 5, 5, 7, 6,
        8, 11, 9, 9, 11, 10,
        12, 13, 15, 13, 14, 15,
        16, 17, 19, 17, 18, 19,
        20, 23, 21, 21, 23, 22
    };

const std::vector<Vertex> Vertices = {
    // pos-Z face
    {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}}, // top right
    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},   // bottom right
    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}}, // bottom left
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}}, // top left
    // neg-Z face
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f},}, // top right
    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f},},  // bottom right
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},}, // bottom left
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f},},// top left
    // pos-Y face
    {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f},},// top right
    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f},},  // bottom right
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f},}, // bottom left
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f},}, // top left
    // neg-Y face
    {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f},}, // top right
    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f},},  // bottom right
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},}, // bottom left
    {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f},},// top left
    // pos-X face
    {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f},}, // top right
    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f},},   // bottom right
    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},}, // bottom left
    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f},}, // top left
    // neg-X face
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f},}, // top right
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f},},   // bottom right
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},},// bottom left
    {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f},},// top left
};

std::unique_ptr<Mesh> Create(uint32_t scaleU, uint32_t scaleV) {
  std::vector<Vertex> verts;
  verts.reserve(Vertices.size());
  for (auto& vert : Vertices) {
    verts.push_back({vert.position, {vert.texCoords.x * scaleU, vert.texCoords.y * scaleV}});
  }
  return std::make_unique<Mesh>(verts, Indices);
}

}

#endif //OPENGL_RENDERER_SRC_RENDERER_SHAPES_CUBE_HPP_
