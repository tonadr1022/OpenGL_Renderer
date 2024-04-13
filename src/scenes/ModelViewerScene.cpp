//
// Created by Tony Adriansen on 3/16/24.
//

#include "ModelViewerScene.hpp"

#include "imgui/imgui.h"
#include "src/Application.hpp"
#include "src/resource/MaterialManager.hpp"
#include "src/resource/ModelManager.hpp"
#include "src/resource/TextureManager.hpp"

ModelViewerScene::ModelViewerScene()
    : Scene({10, 10, 10}, CameraController::Mode::Orbit) {
  m_skyboxNames = {"Sky 1", "Sky 2", "Church", "Winter"};
  m_activeSkyboxName = m_skyboxNames[2];
  Application::Instance().GetRenderer().SetSkyboxTexture(
      TextureManager::GetTexture(HashedString(m_activeSkyboxName.data())));
  //  auto backpack = ModelManager::CopyLoadedModel("backpack");
  //  auto teapot = ModelManager::CopyLoadedModel("teapot");
  //  auto sponza = ModelManager::CopyLoadedModel("sponza");
  auto spot = ModelManager::CopyLoadedModel("spot");

  //  m_modelSelectMap.emplace("Backpack", backpack.get());
  //  m_groups.push_back(std::move(backpack));
  //
  //  m_modelSelectMap.emplace("Teapot", teapot.get());
  //  teapot->transform.SetScale(glm::vec3(1.0));
  //  m_groups.push_back(std::move(teapot));
  //
  //  m_modelSelectMap.emplace("Sponza", sponza.get());
  //  m_groups.push_back(std::move(sponza));

  m_modelSelectMap.emplace("Spot", spot.get());
  spot->transform.SetScale(glm::vec3(10));
  spot->selected = true;
  auto* spot_mat = MaterialManager::GetMaterial("spotTextured");
  for (const auto& obj : spot->GetObjects()) {
    obj->SetMaterial(spot_mat);
  }
  m_groups.push_back(std::move(spot));

  // Lights
  glm::vec3 directional_dir = {0.4f, -0.7f, -0.7f};
  m_directionalLight = std::make_unique<DirectionalLight>(directional_dir);
  m_pointLights.emplace_back(
      std::make_unique<PointLight>(glm::vec3(10, 10, 10)));

  // only one model visible
  for (auto& group : m_groups) {
    group->visible = false;
  }
  m_visibleModel = m_modelSelectMap.find("Spot")->second;
  m_visibleModel->visible = true;
  m_activeModelName = "Spot";
}

void ModelViewerScene::Update(double dt) { Scene::Update(dt); }

void ModelViewerScene::OnImGui() {
  Scene::OnImGui();
  ImGui::Begin("Model Viewer");
  if (ImGui::BeginCombo("##Skybox",
                        ("Skybox: " + m_activeSkyboxName).c_str())) {
    for (auto& name : m_skyboxNames) {
      if (ImGui::Selectable(name.data())) {
        Application::Instance().GetRenderer().SetSkyboxTexture(
            TextureManager::GetTexture(HashedString(name.data())));
        m_activeSkyboxName = name;
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::BeginCombo("##Models", ("Model: " + m_activeModelName).c_str())) {
    for (auto& [name, model] : m_modelSelectMap) {
      if (ImGui::Selectable(name.data())) {
        if (m_visibleModel != nullptr) m_visibleModel->visible = false;
        m_visibleModel = model;
        m_visibleModel->visible = true;
        m_activeModelName = name;
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::CollapsingHeader("Model")) {
    ImGuiTransformComponent(m_visibleModel, "");
  }

  static int selected_object_index = 0;

  if (ImGui::CollapsingHeader("Object Inspector")) {
    if (ImGui::BeginCombo(
            "##ModelObjSel",
            ("Object: " + std::to_string(selected_object_index)).c_str())) {
      for (int i = 0; i < m_visibleModel->GetObjects().size(); i++) {
        const bool is_selected = (selected_object_index == i);
        if (ImGui::Selectable(std::to_string(i).c_str(), is_selected)) {
          selected_object_index = i;
        }
      }
      ImGui::EndCombo();
    }

    if (selected_object_index >= 0 &&
        selected_object_index < m_visibleModel->GetObjects().size()) {
      const auto& obj = m_visibleModel->GetObjects()[selected_object_index];
      ImGui::Checkbox("Visible", &obj->shouldDraw);
      Material* material = obj->GetMaterial();
      ImGui::Text("Material");

      static bool uniform_scale = false;
      ImGui::Checkbox("Uniform##matcolors", &uniform_scale);

      if (uniform_scale) {
        auto old_color = material->diffuseColor;
        if (ImGui::DragFloat3("Diffuse##selObj", &material->diffuseColor.x,
                              0.01, 0, 1)) {
          float change = 0;
          if (old_color.x != material->diffuseColor.x) {
            change = material->diffuseColor.x - old_color.x;
          } else if (old_color.y != material->diffuseColor.y) {
            change = material->diffuseColor.y - old_color.y;
          } else {
            change = material->diffuseColor.z - old_color.z;
          }
          material->diffuseColor = {
              glm::clamp(old_color.x + change, 0.0f, 1.0f),
              glm::clamp(old_color.y + change, 0.0f, 1.0f),
              glm::clamp(old_color.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Diffuse##objsel", &material->diffuseColor.x, 0.01, 0,
                          1);
      }

      if (uniform_scale) {
        auto old_color = material->specularColor;
        if (ImGui::DragFloat3("Specular##selObj", &material->specularColor.x,
                              0.01, 0, 1)) {
          float change = 0;
          if (old_color.x != material->specularColor.x) {
            change = material->specularColor.x - old_color.x;
          } else if (old_color.y != material->specularColor.y) {
            change = material->specularColor.y - old_color.y;
          } else {
            change = material->specularColor.z - old_color.z;
          }
          material->specularColor = {
              glm::clamp(old_color.x + change, 0.0f, 1.0f),
              glm::clamp(old_color.y + change, 0.0f, 1.0f),
              glm::clamp(old_color.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Specular##objsel", &material->specularColor.x, 0.01,
                          0, 1);
      }

      if (uniform_scale) {
        auto old_color = material->ambientColor;
        if (ImGui::DragFloat3("Ambient##selObj", &material->ambientColor.x,
                              0.01, 0, 1)) {
          float change = 0;
          if (old_color.x != material->ambientColor.x) {
            change = material->ambientColor.x - old_color.x;
          } else if (old_color.y != material->ambientColor.y) {
            change = material->ambientColor.y - old_color.y;
          } else {
            change = material->ambientColor.z - old_color.z;
          }
          material->ambientColor = {
              glm::clamp(old_color.x + change, 0.0f, 1.0f),
              glm::clamp(old_color.y + change, 0.0f, 1.0f),
              glm::clamp(old_color.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Ambient##objsel", &material->ambientColor.x, 0.01, 0,
                          1);
      }
      ImGuiTransformComponent(obj.get(), "-1");
    }
  }

  ImGui::End();
}

template <typename T>
void ModelViewerScene::ImGuiTransformComponent(T* obj,
                                               const std::string& iStr) {
  ImGui::Text("Transform");

  auto pos = obj->transform.GetPosition();
  if (ImGui::DragFloat3(("Position##" + iStr).c_str(), &pos.x, 0.01)) {
    obj->transform.SetLocalPos(pos);
  }
  static bool uniform_scale = false;
  ImGui::Checkbox("Uniform##scale", &uniform_scale);
  auto scale = obj->transform.GetScale();
  if (uniform_scale) {
    auto old_scale = scale;
    if (ImGui::DragFloat3(("Scale##" + iStr).c_str(), &scale.x, 0.001)) {
      float change = 0;
      if (old_scale.x != scale.x) {
        change = scale.x - old_scale.x;
      } else if (old_scale.y != scale.y) {
        change = scale.y - old_scale.y;
      } else {
        change = scale.z - old_scale.z;
      }
      obj->transform.SetScale(glm::vec3(
          old_scale.x + change, old_scale.y + change, old_scale.z + change));
    }
  } else {
    if (ImGui::DragFloat3(("Scale##" + iStr).c_str(), &scale.x, 0.001)) {
      obj->transform.SetScale(scale);
    }
  }
}
