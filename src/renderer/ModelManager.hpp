//
// Created by Tony Adriansen on 3/19/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_MODELMANAGER_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_MODELMANAGER_HPP_

#include <string>
#include "src/renderer/group/Group.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelManager {
 public:
  static void LoadModel(HashedString modelName, const std::string& modelPath, bool backfaceCull = true);
  static std::unique_ptr<Group> CopyLoadedModel(HashedString modelName);
 private:
  static void ProcessNodes(aiNode* rootNode, const aiScene* scene);
  static void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<bool>& processedMaterials);
  static Material* LoadMaterial(aiMaterial* aiMat, HashedString matName);
  static std::vector<TexturePair> LoadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiType, MatTextureType matTextureType);

  static inline std::string path;
  static inline std::string directory;
  static inline std::string name;
  static inline Group* group;
  static inline std::unordered_map<HashedString, std::unique_ptr<Group>> groups;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_MODELMANAGER_HPP_
