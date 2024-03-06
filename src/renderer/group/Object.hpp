//
// Created by Tony Adriansen on 3/1/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_OBJECT_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_OBJECT_HPP_

#include <memory>
#include "src/renderer/Mesh.hpp"
#include "src/renderer/Transform.hpp"
#include "src/renderer/Material.hpp"

class Object {
 public:
  Object(const Mesh* mesh, Material* material);

  Transform transform;
  void SetMaterial(Material* material);
  void SetMesh(const Mesh* mesh);
  const Material* GetMaterial() const;
  const Mesh* GetMesh() const;

 protected:
  const Material* m_material = nullptr;
  const Mesh* m_mesh = nullptr;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_OBJECT_HPP_
