//
// Created by Tony Adriansen on 3/11/24.
//

#include "Model.hpp"
#include "src/core/Logger.hpp"
#include <stack>
#include "src/renderer/resource/MeshManager.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/renderer/resource/MaterialManager.hpp"

Model::Model(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    LOG_ERROR("Assimp Error: %s", importer.GetErrorString());
    return;
  }
  uint32_t slashIdx = path.find_last_of('/');
  m_directory = path.substr(0, slashIdx);
  m_name = path.substr(slashIdx + 1);
  ProcessNodes(scene->mRootNode, scene);
}

void Model::ProcessNodes(aiNode* rootNode, const aiScene* scene) {
  std::stack<aiNode*> meshStack;
  meshStack.push(rootNode);
  aiNode* currNode;
  while (!meshStack.empty()) {
    currNode = meshStack.top();
    meshStack.pop();
    // process and add meshes of the current node
    for (uint32_t i = 0; i < currNode->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[currNode->mMeshes[i]];
      ProcessMesh(mesh, scene);
    }
    // push children to the stack
    for (uint32_t i = 0; i < currNode->mNumChildren; i++) {
      meshStack.push(currNode->mChildren[i]);
    }
  }
}

void Model::ProcessMesh(aiMesh* aiMesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  std::vector<Texture*> textures;

  // process vertices
  for (uint32_t i = 0; i < aiMesh->mNumVertices; i++) {
    Vertex v;
    glm::vec3 tmp;
    tmp.x = aiMesh->mVertices[i].x;
    tmp.y = aiMesh->mVertices[i].y;
    tmp.z = aiMesh->mVertices[i].z;
    v.position = tmp;
    tmp.x = aiMesh->mNormals[i].x;
    tmp.y = aiMesh->mNormals[i].y;
    tmp.z = aiMesh->mNormals[i].z;
    v.normal = tmp;

    // Assimp allows up to 8 texture coordinates per vertex, only using first for now
    if (aiMesh->mTextureCoords[0]) {
      glm::vec2 tmp;
      tmp.x = aiMesh->mTextureCoords[0][i].x;
      tmp.y = aiMesh->mTextureCoords[0][i].y;
      v.texCoords = tmp;
    } else {
      v.texCoords = glm::vec2(0, 0);
    }

    vertices.push_back(v);
  }

  // process indices
  for (uint32_t i = 0; i < aiMesh->mNumFaces; i++) {
    aiFace face = aiMesh->mFaces[i];
    for (uint32_t j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // process material if mesh has one
  if (aiMesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
    auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Type::Diffuse);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Type::Specular);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  auto meshMatName = HashedString((m_name + std::to_string(m_objects.size())).c_str());
  Mesh* mesh = MeshManager::AddMesh(meshMatName, vertices, indices);
  MaterialManager::AddMaterial(meshMatName, textures, "blinnPhong", Material::Type::BlinnPhong);
  m_objects.push_back(std::make_unique<Object>(mesh, MaterialManager::GetMaterial(meshMatName)));
}

std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial* material,
                                                  aiTextureType aiType,
                                                  Texture::Type textureType) {
  std::vector<Texture*> textures;
  for (uint32_t i = 0; i < material->GetTextureCount(aiType); i++) {
    aiString textureFilename;
    material->GetTexture(aiType, i, &textureFilename);


    // add or get the texture
//    auto t = std::string(m_name + textureFilename.data);
    auto textureName = HashedString((m_name + textureFilename.data).c_str());
//    std::cout << textureName.data() << std::endl;
    Texture* texture = TextureManager::GetTexture(textureName);
    if (texture == nullptr) {
      texture = TextureManager::AddTexture(textureName, m_directory + '/' + textureFilename.data, textureType);
    }
    textures.push_back(texture);
  }

  return textures;
}
