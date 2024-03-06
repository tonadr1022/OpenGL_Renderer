//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_SHAPES_CUBE_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_SHAPES_CUBE_HPP_

#include "src/renderer/Mesh.hpp"
#include <memory>

namespace Cube {

const std::vector<uint32_t> Indices =
    {0, 1, 3, 1, 2, 3,
     4, 5, 7, 5, 6, 7,
     8, 9, 11, 9, 10, 11,
     12, 13, 15, 13, 14, 15,
     16, 17, 19, 17, 18, 19,
     20, 21, 23, 21, 22, 23
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

std::shared_ptr<Mesh> Create() {
  return std::make_shared<Mesh>(Vertices, Indices);
}

}

#endif //OPENGL_RENDERER_SRC_RENDERER_SHAPES_CUBE_HPP_
