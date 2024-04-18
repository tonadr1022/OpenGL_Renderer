//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GROUP_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GROUP_HPP_

#include <memory>
#include <vector>

#include "src/group/Object.hpp"

class Group {
  friend class ModelManager;
  // for instancing demo for now
  friend class Application;

 public:
  explicit Group(bool backFaceCull = true);
  void AddObject(std::unique_ptr<Object> object);
  void RemoveObject(const Object* object);

  Group(Group& other) {
    this->backFaceCull = other.backFaceCull;
    this->stencil = other.stencil;
    for (const auto& object : other.m_objects) {
      this->m_objects.push_back(std::make_unique<Object>(*object));
    }
    this->transform = other.transform;
  }

  void UpdateTransforms();
  [[nodiscard]] inline const std::vector<std::unique_ptr<Object>>& GetObjects() const {
    return m_objects;
  }

  Transform transform;
  bool reflective = false;
  bool backFaceCull = true;
  bool stencil = false;
  bool visible = true;
  bool wireframe = false;

 protected:
  std::vector<std::unique_ptr<Object>> m_objects;
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_GROUP_HPP_
