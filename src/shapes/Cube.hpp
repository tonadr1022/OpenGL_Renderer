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
        0, 3, 1, 1, 3, 2,
        4, 5, 7, 5, 6, 7,
        8, 9, 11, 9, 10, 11,
        12, 15, 13, 13, 15, 14,
        16, 19, 17, 17, 19, 18,
        20, 21, 23, 21, 22, 23
    };

const std::vector<Vertex> Vertices = {
    // pos-Z face
    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // top right
    {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},   // bottom right
    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // bottom left
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // top left
    // neg-Z face
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f},}, // top right
    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f},},  // bottom right
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f},}, // bottom left
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f},},// top left
    // pos-Y face
    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f},},// top right
    {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f},},  // bottom right
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f},}, // bottom left
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f},}, // top left
    // neg-Y face
    {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f},}, // top right
    {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f},},  // bottom right
    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f},}, // bottom left
    {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f},},// top left
    // pos-X face
    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f},}, // top right
    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f},},   // bottom right
    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},}, // bottom left
    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f},}, // top left
    // neg-X face
    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f},}, // top right
    {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f},},   // bottom right
    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},},// bottom left
    {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f},},// top left
};

std::unique_ptr<Mesh> Create(uint32_t scaleU, uint32_t scaleV) {
  if (scaleU != 1 || scaleV != 1) {
    std::vector<Vertex> scaledVertices;
    scaledVertices.reserve(Vertices.size());
    for (auto& vert : Vertices) {
      scaledVertices.push_back({vert.position, vert.normal, {vert.texCoords.x * scaleU, vert.texCoords.y * scaleV}});
    }
    return std::make_unique<Mesh>(scaledVertices, Indices);
  } else {
    return std::make_unique<Mesh>(Vertices, Indices);
  }
}

}

#endif //OPENGL_RENDERER_SRC_RENDERER_SHAPES_CUBE_HPP_
