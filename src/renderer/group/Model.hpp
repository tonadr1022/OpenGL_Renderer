//
// Created by Tony Adriansen on 3/11/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GROUP_MODEL_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GROUP_MODEL_HPP_

#include "Group.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Group {
 public:
  explicit Model(const std::string& path);

 private:
  std::string m_directory;
  std::string m_name;
  void ProcessNodes(aiNode* rootNode, const aiScene* scene);
  void ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture*> LoadMaterialTextures(aiMaterial* material, aiTextureType aiType, Texture::Type textureType);


};

#endif //OPENGL_RENDERER_SRC_RENDERER_GROUP_MODEL_HPP_
