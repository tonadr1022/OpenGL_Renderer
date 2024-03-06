//
// Created by Tony Adriansen on 3/1/24.
//

#include "Object.hpp"
#include "src/core/Logger.hpp"


Object::Object(const Mesh* mesh, Material* material)
    : m_material(material), m_mesh(mesh) {}

void Object::SetMaterial(Material* material) {
  m_material = material;
}

void Object::SetMesh(const Mesh* mesh) {
  m_mesh = mesh;
}

const Mesh* Object::GetMesh() const {
  return m_mesh;
}

const Material* Object::GetMaterial() const {
  return m_material;
}
