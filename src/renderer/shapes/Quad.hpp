//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_SHAPES_QUAD_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_SHAPES_QUAD_HPP_

#include "src/renderer/Mesh.hpp"
#include <memory>

namespace Quad {

const std::vector<uint32_t> Indices = {0, 1, 3, 1, 2, 3};

const std::vector<Vertex> Vertices = {
    Vertex{0.5f, 0.5f, 0.0f, 1.0f, 1.0f}, // top right
    Vertex{0.5f, -0.5f, 0.0f, 1.0f, 0.0f},   // bottom right
    Vertex{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f}, // bottom left
    Vertex{-0.5f, 0.5f, 0.0f, 0.0f, 1.0f}, // top left
};

std::shared_ptr<Mesh> Create() {
  return std::make_shared<Mesh>(Vertices, Indices);
}
}
#endif //OPENGL_RENDERER_SRC_RENDERER_SHAPES_QUAD_HPP_
