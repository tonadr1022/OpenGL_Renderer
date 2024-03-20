//
// Created by Tony Adriansen on 3/19/24.
//

#include "ModelManager.hpp"
#include "src/core/Logger.hpp"
#include <stack>
#include "src/renderer/resource/MeshManager.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/renderer/resource/MaterialManager.hpp"

std::unique_ptr<Group> ModelManager::CopyLoadedModel(HashedString modelName) {
  auto it = groups.find(modelName);
  if (it == groups.end()) {
    LOG_ERROR("Model not found");
    return nullptr;
  }
  return std::make_unique<Group>(*it->second);
}

void ModelManager::LoadModel(HashedString modelName, const std::string& modelPath, bool backfaceCull) {
  auto model = std::make_unique<Group>();
  ModelManager::path = modelPath;
  uint32_t slashIdx = modelPath.find_last_of('/');
  ModelManager::directory = modelPath.substr(0, slashIdx);
  group = model.get();
  group->backFaceCull = backfaceCull;

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
      | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    LOG_ERROR("Assimp Error: %s", importer.GetErrorString());
    return;
  }
  LOG_INFO("Mats: %i, meshes: %i, textures: %i ", scene->mNumMaterials, scene->mNumMeshes, scene->mNumTextures);
  ProcessNodes(scene->mRootNode, scene);
  groups.emplace(modelName, std::move(model));

}

void ModelManager::ProcessNodes(aiNode* rootNode, const aiScene* scene) {
  std::stack<aiNode*> meshStack;
  meshStack.push(rootNode);
  aiNode* currNode;
  std::vector<bool> processedMaterials;
  processedMaterials.resize(scene->mNumMaterials);
  while (!meshStack.empty()) {
    currNode = meshStack.top();
    meshStack.pop();
    // process and add meshes of the current node
    for (uint32_t i = 0; i < currNode->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[currNode->mMeshes[i]];
      ProcessMesh(mesh, scene, processedMaterials);
    }
    // push children to the stack
    for (uint32_t i = 0; i < currNode->mNumChildren; i++) {
      meshStack.push(currNode->mChildren[i]);
    }
  }
}

void ModelManager::ProcessMesh(aiMesh* aiMesh, const aiScene* scene, std::vector<bool>& processedMaterials) {
  std::vector<Vertex> vertices;
  vertices.reserve(aiMesh->mNumVertices);
  std::vector<uint32_t> indices;

  for (uint32_t i = 0; i < aiMesh->mNumVertices; i++) {
    Vertex v;

    if (aiMesh->HasPositions()) {
      v.position = glm::vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
    }
    if (aiMesh->HasNormals()) {
      v.normal = glm::vec3(aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);
    }
    if (aiMesh->HasTextureCoords(0)) {
      v.texCoords = glm::vec2(aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y);
    } else {
      v.texCoords = glm::vec2(0);
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
  auto meshName = HashedString((std::string(path) + std::to_string(group->m_objects.size())).c_str());
  Mesh* mesh = MeshManager::AddMesh(meshName, vertices, indices);

  // process material if mesh has one and hasn't been made already
  if (aiMesh->mMaterialIndex >= 0) {
    Material* mat = nullptr;
    auto matName = HashedString((std::string(path) + std::to_string(aiMesh->mMaterialIndex)).c_str());
    if (processedMaterials[aiMesh->mMaterialIndex]) {
      mat = MaterialManager::GetMaterial(matName);
    } else {
      aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
      mat = LoadMaterial(aiMat, matName);
      processedMaterials[aiMesh->mMaterialIndex] = true;
    }
    group->m_objects.push_back(std::make_unique<Object>(mesh, mat));
  } else {
    group->m_objects.push_back(std::make_unique<Object>(mesh));
  }
}

Material* ModelManager::LoadMaterial(aiMaterial* aiMat, HashedString matName) {
  std::vector<TexturePair> textures;
  auto diffuseMaps = LoadMaterialTextures(aiMat, aiTextureType_DIFFUSE, MatTextureType::Diffuse);
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
  auto specularMaps = LoadMaterialTextures(aiMat, aiTextureType_SPECULAR, MatTextureType::Specular);
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  MaterialManager::AddMaterial(matName, textures, "blinnPhong", Material::Type::BlinnPhong);
  Material* mat = MaterialManager::GetMaterial(matName);

  aiColor4D color;
  if (AI_SUCCESS == aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &color)) {
    mat->diffuseColor = glm::vec3(color.r, color.g, color.b);
  }
  if (AI_SUCCESS == aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_SPECULAR, &color)) {
    mat->specularColor = glm::vec3(color.r, color.g, color.b);
  }
  if (AI_SUCCESS == aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &color)) {
    mat->ambientColor = glm::vec3(color.r, color.g, color.b);
  }
  ai_real shininess;
  int res1;
  res1 = aiGetMaterialFloat(aiMat, AI_MATKEY_SHININESS, &shininess);
  if (res1 == AI_SUCCESS && shininess > 0) {
    mat->shininess = shininess;
  }
  return mat;
}
std::vector<TexturePair> ModelManager::LoadMaterialTextures(aiMaterial* aiMaterial,
                                                            aiTextureType aiType,
                                                            MatTextureType matTextureType) {
  std::vector<TexturePair> textures;
  for (uint32_t i = 0; i < aiMaterial->GetTextureCount(aiType); i++) {
    aiString textureFilename;
    aiMaterial->GetTexture(aiType, i, &textureFilename);


    // add or get the texture
//    auto t = std::string(m_name + textureFilename.data);
    auto textureName = HashedString((std::string(path) + textureFilename.data).c_str());
//    std::cout << m_name << " " << textureFilename.data << std::endl;
    Texture* texture = TextureManager::GetTexture(textureName);
    if (texture == nullptr) {
      texture = TextureManager::AddTexture(textureName,
                                           std::string(directory) + '/' + textureFilename.data,
                                           Texture::SamplerType::TwoD);
    }
    textures.emplace_back(matTextureType, texture);
  }
  return textures;
}
