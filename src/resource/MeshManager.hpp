//
// Created by Tony Adriansen on 3/3/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_RESOURCE_MESHMANAGER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_RESOURCE_MESHMANAGER_HPP_

#include "src/renderer/Mesh.hpp"
#include <unordered_map>
#include "src/utils/HashedString.hpp"


class MeshManager {
 public:
  template<typename... Args>
  static Mesh* AddMesh(HashedString name, Args&& ... args) {
    m_meshes.emplace(name, std::make_unique<Mesh>(std::forward<Args>(args)...));
    return m_meshes.at(name).get();
  }

  static Mesh* AddMesh(HashedString name, std::unique_ptr<Mesh> mesh);

  static void RemoveMesh(HashedString name);
  static Mesh* GetMesh(HashedString name);
 private:
  static std::unordered_map<HashedString, std::unique_ptr<Mesh>> m_meshes;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_RESOURCE_MESHMANAGER_HPP_
