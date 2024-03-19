//
// Created by Tony Adriansen on 3/11/24.
//

#include "Model.hpp"
#include "src/core/Logger.hpp"
#include <stack>
#include "src/renderer/resource/MeshManager.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/renderer/resource/MaterialManager.hpp"

Model::Model(const std::string& path, bool backfaceCull) : Group(backfaceCull) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
      | aiProcess_CalcTangentSpace);
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
  auto meshName = HashedString((m_name + std::to_string(m_objects.size())).c_str());
  Mesh* mesh = MeshManager::AddMesh(meshName, vertices, indices);

  // process material if mesh has one and hasn't been made already
  if (aiMesh->mMaterialIndex >= 0) {
    auto matName = HashedString((m_name + std::to_string(aiMesh->mMaterialIndex)).c_str());

    Material* existingMat = MaterialManager::GetMaterial(matName);
    if (existingMat == nullptr) {
//      LOG_INFO("index %i", aiMesh->mMaterialIndex);
      aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
      Material* mat = LoadMaterial(aiMat, matName);
      m_objects.push_back(std::make_unique<Object>(mesh, mat));
//    LOG_INFO("Mesh vertices: %d, Indices: %d, Textures: %d", vertices.size(), indices.size(), textures.size());
    } else {
      m_objects.push_back(std::make_unique<Object>(mesh, existingMat));
    }
  } else {
    m_objects.push_back(std::make_unique<Object>(mesh));
  }
}

std::vector<TexturePair> Model::LoadMaterialTextures(aiMaterial* aiMaterial,
                                                     aiTextureType aiType,
                                                     MatTextureType matTextureType) {
  std::vector<TexturePair> textures;
  for (uint32_t i = 0; i < aiMaterial->GetTextureCount(aiType); i++) {
    aiString textureFilename;
    aiMaterial->GetTexture(aiType, i, &textureFilename);


    // add or get the texture
//    auto t = std::string(m_name + textureFilename.data);
    auto textureName = HashedString((m_name + textureFilename.data).c_str());
//    std::cout << m_name << " " << textureFilename.data << std::endl;
    Texture* texture = TextureManager::GetTexture(textureName);
    if (texture == nullptr) {
      texture =
          TextureManager::AddTexture(textureName, m_directory + '/' + textureFilename.data, Texture::SamplerType::TwoD);
    }
    textures.emplace_back(matTextureType, texture);
  }

  return textures;
}

Material* Model::LoadMaterial(aiMaterial* aiMat, HashedString matName) {
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
