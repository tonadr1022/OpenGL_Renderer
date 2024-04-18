#include "src/scenes/InstancingScene.hpp"

#include <cstdint>
#include <memory>

#include "src/camera/CameraController.hpp"
#include "src/group/Object.hpp"
#include "src/resource/MaterialManager.hpp"
#include "src/resource/MeshManager.hpp"

InstancingScene::InstancingScene() : Scene({50, 50, 50}, "Sky 1", CameraController::Mode::Orbit) {
  // unsigned int amount = 10000;
  // std::vector<glm::mat4> model_matrices(amount);
  // // from learn opengl, I take no credit, just trying to get it working
  // srand(static_cast<unsigned int>(glfwGetTime()));  // initialize random seed
  // float radius = 150.0;
  // float offset = 25.0f;
  //
  // for (unsigned int i = 0; i < amount; i++) {
  //   auto model = glm::mat4(1.0f);
  //   // 1. translation: displace along circle with 'radius' in range [-offset, offset]
  //   // float angle = static_cast<float>(i) / static_cast<float>(amount) * 360.0f;
  //   float angle = 0;
  //   // float displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
  //   float displacement = i;
  //   float x = sin(angle) * radius + displacement;
  //   displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
  //   // keep height of asteroid field smaller compared to width of x and
  //   float y = displacement * 0.4f;
  //   displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
  //   float z = cos(angle) * radius + displacement;
  //   model = glm::translate(model, glm::vec3(x, y, z));
  //
  //   // 2. scale: Scale between 0.05 and 0.25f
  //   auto scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
  //   // auto scale = 0.001;
  //   model = glm::scale(model, glm::vec3(scale));
  //
  //   // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
  //   auto rot_angle = static_cast<float>((rand() % 360));
  //   model = glm::rotate(model, rot_angle, glm::vec3(0.4f, 0.6f, 0.8f));
  //
  //   // 4. now add to list of matrices
  //   model_matrices[i] = model;
  // }

  glm::vec3 dims = {100, 100, 100};
  uint32_t amount = dims.x * dims.y * dims.z;
  int i = 0;
  m_model_matrices.reserve(amount);
  for (int y = -dims.y / 2; y < dims.y / 2; y++) {
    for (int z = -dims.z / 2; z < dims.z / 2; z++) {
      for (int x = -dims.x / 2; x < dims.x / 2; x++, i++) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x * 10, y * 10, z * 10));
        m_model_matrices.push_back(model);
      }
    }
  }

  // enable instance matrix attributes.

  Material* wood_container_mat = MaterialManager::GetMaterial("woodContainer");
  Mesh* cube_mesh = MeshManager::GetMesh("cube");
  auto cube = std::make_unique<Object>(cube_mesh, wood_container_mat);
  // auto new_spot = ModelManager::CopyLoadedModel("cube");
  auto new_spot = std::make_unique<Group>();
  new_spot->AddObject(std::move(cube));

  // auto* spot_mat = MaterialManager::GetMaterial("spotTextured");
  // for (const auto& obj : new_spot->GetObjects()) {
  //   obj->SetMaterial(spot_mat);
  // }
  m_instanced_models.push_back(std::move(new_spot));
  m_instanced_model_renderers.emplace_back(
      std::make_unique<InstancedModelRenderer>(m_instanced_models[0].get(), m_model_matrices));

  glm::vec3 directional_dir = {0.4f, -0.7f, -0.7f};
  m_directionalLight = std::make_unique<DirectionalLight>(directional_dir);
}

void InstancingScene::Update(double dt) {}
