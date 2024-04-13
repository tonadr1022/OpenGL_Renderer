//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_OBJECT_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_OBJECT_HPP_

#include "Transform.hpp"
#include "src/renderer/Material.hpp"
#include "src/renderer/Mesh.hpp"

class Object {
 public:
  Object(const Mesh* mesh, Material* material);
  explicit Object(const Mesh* mesh);

  Transform transform;
  void SetMaterial(Material* material);
  void SetMesh(const Mesh* mesh);
  [[nodiscard]] bool HasMaterial() const;
  [[nodiscard]] Material* GetMaterial() const;
  [[nodiscard]] const Mesh* GetMesh() const;
  bool shouldDraw = true;
  bool cameraVisible = true;

 protected:
  Material* m_material = nullptr;
  const Mesh* m_mesh = nullptr;
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_OBJECT_HPP_
