//
// Created by Tony Adriansen on 3/3/24.
//

#include "MeshManager.hpp"
#include "src/utils/Logger.hpp"

std::unordered_map<HashedString, std::unique_ptr<Mesh>> MeshManager::m_meshes;

Mesh* MeshManager::GetMesh(HashedString name) {
  auto it = m_meshes.find(name);
  if (it == m_meshes.end()) {
    LOG_ERROR("Mesh not found: %s", name.data());
    return nullptr;
  }
  return it->second.get();
}

void MeshManager::RemoveMesh(HashedString name) {
  auto it = m_meshes.find(name);
  if (it == m_meshes.end()) {
    LOG_ERROR("Mesh not found, cannot remove: %s", name.data());
    return;
  }
  m_meshes.erase(it);
}

Mesh* MeshManager::AddMesh(HashedString name, std::unique_ptr<Mesh> mesh) {
  if (m_meshes.find(name) != m_meshes.end()) LOG_ERROR("Mesh of name %s already found", name.data());
  auto it = m_meshes.emplace(name, std::move(mesh));
  return it.first->second.get();
}