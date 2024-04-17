//
// Created by Tony Adriansen on 3/19/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_MODELMANAGER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_MODELMANAGER_HPP_

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>

#include "src/group/Group.hpp"
#include "src/utils/HashedString.hpp"

class ModelManager {
 public:
  static void LoadModel(HashedString modelName, const std::string& modelPath,
                        bool backfaceCull = true);
  static std::unique_ptr<Group> CopyLoadedModel(HashedString modelName);
  static Group* GetModel(HashedString modelName);
  static Group* CopyModel(HashedString existingModelName, HashedString newModelName);

 private:
  static void ProcessNodes(aiNode* rootNode, const aiScene* scene);
  static void ProcessMesh(aiMesh* mesh, const aiScene* scene,
                          std::vector<bool>& processedMaterials);
  static Material* LoadMaterial(aiMaterial* aiMat, HashedString matName);
  static std::vector<TexturePair> LoadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiType,
                                                       MatTextureType matTextureType);

  static inline std::string m_path;
  static inline std::string m_directory;
  static inline std::string m_name;
  static inline Group* m_group;
  static inline std::unordered_map<HashedString, std::unique_ptr<Group>> m_groups;
};

#endif  // OPENGL_RENDERER_SRC_RENDERER_MODELMANAGER_HPP_
