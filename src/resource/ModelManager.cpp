//
// Created by Tony Adriansen on 3/19/24.
//

#include "ModelManager.hpp"

#include <stack>

#include "src/resource/MaterialManager.hpp"
#include "src/resource/MeshManager.hpp"
#include "src/resource/TextureManager.hpp"
#include "src/utils/Logger.hpp"

std::unique_ptr<Group> ModelManager::CopyLoadedModel(HashedString modelName) {
  auto it = m_groups.find(modelName);
  if (it == m_groups.end()) {
    LOG_ERROR("Model not found");
    return nullptr;
  }
  return std::make_unique<Group>(*it->second);
}

void ModelManager::LoadModel(HashedString modelName, const std::string& modelPath,
                             bool backfaceCull) {
  auto model = std::make_unique<Group>();
  ModelManager::m_path = modelPath;
  uint32_t slash_idx = modelPath.find_last_of('/');
  ModelManager::m_directory = modelPath.substr(0, slash_idx);
  m_group = model.get();
  m_group->backFaceCull = backfaceCull;

  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs |
                                       aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    LOG_ERROR("Assimp Error: %s", importer.GetErrorString());
    return;
  }
  LOG_INFO("Mats: %i, meshes: %i, textures: %i ", scene->mNumMaterials, scene->mNumMeshes,
           scene->mNumTextures);
  ProcessNodes(scene->mRootNode, scene);
  m_groups.emplace(modelName, std::move(model));
}

void ModelManager::ProcessNodes(aiNode* rootNode, const aiScene* scene) {
  std::stack<aiNode*> mesh_stack;
  mesh_stack.push(rootNode);
  aiNode* curr_node;
  std::vector<bool> processed_materials;
  processed_materials.resize(scene->mNumMaterials);
  while (!mesh_stack.empty()) {
    curr_node = mesh_stack.top();
    mesh_stack.pop();
    // process and add meshes of the current node
    for (uint32_t i = 0; i < curr_node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[curr_node->mMeshes[i]];
      ProcessMesh(mesh, scene, processed_materials);
    }
    // push children to the stack
    for (uint32_t i = 0; i < curr_node->mNumChildren; i++) {
      mesh_stack.push(curr_node->mChildren[i]);
    }
  }
}

void ModelManager::ProcessMesh(aiMesh* aiMesh, const aiScene* scene,
                               std::vector<bool>& processedMaterials) {
  std::vector<Vertex> vertices;
  vertices.reserve(aiMesh->mNumVertices);
  std::vector<uint32_t> indices;

  for (uint32_t i = 0; i < aiMesh->mNumVertices; i++) {
    Vertex v;

    if (aiMesh->HasPositions()) {
      v.position =
          glm::vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
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
  auto mesh_name =
      HashedString((std::string(m_path) + std::to_string(m_group->m_objects.size())).c_str());
  Mesh* mesh = MeshManager::AddMesh(mesh_name, vertices, indices);

  // process material if mesh has one and hasn't been made already
  if (aiMesh->mMaterialIndex >= 0) {
    Material* mat = nullptr;
    auto mat_name =
        HashedString((std::string(m_path) + std::to_string(aiMesh->mMaterialIndex)).c_str());
    if (processedMaterials[aiMesh->mMaterialIndex]) {
      mat = MaterialManager::GetMaterial(mat_name);
    } else {
      aiMaterial* ai_mat = scene->mMaterials[aiMesh->mMaterialIndex];
      mat = LoadMaterial(ai_mat, mat_name);
      processedMaterials[aiMesh->mMaterialIndex] = true;
    }
    m_group->m_objects.push_back(std::make_unique<Object>(mesh, mat));
  } else {
    m_group->m_objects.push_back(std::make_unique<Object>(mesh));
  }
}

Material* ModelManager::LoadMaterial(aiMaterial* aiMat, HashedString matName) {
  std::vector<TexturePair> textures;
  auto diffuse_maps = LoadMaterialTextures(aiMat, aiTextureType_DIFFUSE, MatTextureType::Diffuse);
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
  auto specular_maps =
      LoadMaterialTextures(aiMat, aiTextureType_SPECULAR, MatTextureType::Specular);
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

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
    aiString texture_filename;
    aiMaterial->GetTexture(aiType, i, &texture_filename);

    // add or get the texture
    //    auto t = std::string(m_name + textureFilename.data);
    auto texture_name = HashedString((std::string(m_path) + texture_filename.data).c_str());
    //    std::cout << m_name << " " << textureFilename.data << std::endl;
    Texture* texture = TextureManager::GetTexture(texture_name);
    if (texture == nullptr) {
      texture = TextureManager::AddTexture(texture_name,
                                           std::string(m_directory) + '/' + texture_filename.data,
                                           Texture::SamplerType::TwoD);
    }
    textures.emplace_back(matTextureType, texture);
  }
  return textures;
}
