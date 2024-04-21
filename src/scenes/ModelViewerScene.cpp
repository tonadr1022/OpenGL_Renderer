//
// Created by Tony Adriansen on 3/16/24.
//

#include "ModelViewerScene.hpp"

#include "imgui/imgui.h"
#include "src/resource/MaterialManager.hpp"
#include "src/resource/ModelManager.hpp"
#include "src/utils/HashedString.hpp"

ModelViewerScene::ModelViewerScene() : Scene({50, 50, 50}, "Sky 1", CameraController::Mode::Orbit) {
  m_skyboxNames = {"Sky 1", "Sky 2", "Church", "Winter"};
  std::vector<std::pair<HashedString, float>> names_scales = {
      {"teapot", 0.1}, {"sponza", 0.1}, {"spot", 10},
      {"dragon", 1},   {"backpack", 1}, {"learn_opengl_rock", 1}};
  for (const auto& pair : names_scales) {
    auto obj = ModelManager::CopyLoadedModel(pair.first);
    obj->transform.SetScale(glm::vec3(pair.second));
    m_modelSelectMap.emplace(pair.first, obj.get());
    m_groups.push_back(std::move(obj));
  }

  auto* oak_mat = MaterialManager::GetMaterial("oak");
  for (const auto& obj : m_modelSelectMap.at("teapot")->GetObjects()) {
    obj->SetMaterial(oak_mat);
  }

  auto* spot_mat = MaterialManager::GetMaterial("spotTextured");
  for (const auto& obj : m_modelSelectMap.at("spot")->GetObjects()) {
    obj->SetMaterial(spot_mat);
  }

  // Lights
  glm::vec3 directional_dir = {0.4f, -0.7f, -0.7f};
  m_directionalLight = std::make_unique<DirectionalLight>(directional_dir);
  m_pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3(10, 10, 10)));

  // only one model visible
  for (auto& group : m_groups) {
    group->visible = false;
  }
  m_visibleModel = m_modelSelectMap.find("spot")->second;
  m_visibleModel->visible = true;
  m_activeModelName = "spot";
}

void ModelViewerScene::Update(double dt) { Scene::Update(dt); }

void ModelViewerScene::OnImGui() {
  Scene::OnImGui();
  ImGui::Begin("Model Viewer");
  if (ImGui::BeginCombo("##Skybox", ("Skybox: " + m_skyboxName).c_str())) {
    for (auto& name : m_skyboxNames) {
      if (ImGui::Selectable(name.data())) {
        m_skyboxName = name;
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

  if (ImGui::Checkbox("Stencil Outline", &m_stencilSelectedDemo)) {
    m_visibleModel->stencil = m_stencilSelectedDemo;
  }

  ImGui::Checkbox("Reflective", &m_visibleModel->reflective);

  static int selected_object_index = 0;

  if (ImGui::CollapsingHeader("Object Inspector")) {
    if (ImGui::BeginCombo("##ModelObjSel",
                          ("Object: " + std::to_string(selected_object_index)).c_str())) {
      for (int i = 0; i < m_visibleModel->GetObjects().size(); i++) {
        const bool is_selected = (selected_object_index == i);
        if (ImGui::Selectable(std::to_string(i).c_str(), is_selected)) {
          selected_object_index = i;
        }
      }
      ImGui::EndCombo();
    }

    if (selected_object_index >= 0 && selected_object_index < m_visibleModel->GetObjects().size()) {
      const auto& obj = m_visibleModel->GetObjects()[selected_object_index];
      ImGui::Checkbox("Visible", &obj->shouldDraw);
      Material* material = obj->GetMaterial();
      ImGui::Text("Material");

      static bool uniform_scale = false;
      ImGui::Checkbox("Uniform##matcolors", &uniform_scale);

      if (uniform_scale) {
        auto old_color = material->diffuseColor;
        if (ImGui::DragFloat3("Diffuse##selObj", &material->diffuseColor.x, 0.01, 0, 1)) {
          float change = 0;
          if (old_color.x != material->diffuseColor.x) {
            change = material->diffuseColor.x - old_color.x;
          } else if (old_color.y != material->diffuseColor.y) {
            change = material->diffuseColor.y - old_color.y;
          } else {
            change = material->diffuseColor.z - old_color.z;
          }
          material->diffuseColor = {glm::clamp(old_color.x + change, 0.0f, 1.0f),
                                    glm::clamp(old_color.y + change, 0.0f, 1.0f),
                                    glm::clamp(old_color.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Diffuse##objsel", &material->diffuseColor.x, 0.01, 0, 1);
      }

      if (uniform_scale) {
        auto old_color = material->specularColor;
        if (ImGui::DragFloat3("Specular##selObj", &material->specularColor.x, 0.01, 0, 1)) {
          float change = 0;
          if (old_color.x != material->specularColor.x) {
            change = material->specularColor.x - old_color.x;
          } else if (old_color.y != material->specularColor.y) {
            change = material->specularColor.y - old_color.y;
          } else {
            change = material->specularColor.z - old_color.z;
          }
          material->specularColor = {glm::clamp(old_color.x + change, 0.0f, 1.0f),
                                     glm::clamp(old_color.y + change, 0.0f, 1.0f),
                                     glm::clamp(old_color.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Specular##objsel", &material->specularColor.x, 0.01, 0, 1);
      }

      if (uniform_scale) {
        auto old_color = material->ambientColor;
        if (ImGui::DragFloat3("Ambient##selObj", &material->ambientColor.x, 0.01, 0, 1)) {
          float change = 0;
          if (old_color.x != material->ambientColor.x) {
            change = material->ambientColor.x - old_color.x;
          } else if (old_color.y != material->ambientColor.y) {
            change = material->ambientColor.y - old_color.y;
          } else {
            change = material->ambientColor.z - old_color.z;
          }
          material->ambientColor = {glm::clamp(old_color.x + change, 0.0f, 1.0f),
                                    glm::clamp(old_color.y + change, 0.0f, 1.0f),
                                    glm::clamp(old_color.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Ambient##objsel", &material->ambientColor.x, 0.01, 0, 1);
      }
      ImGuiTransformComponent(obj.get(), "-1");
    }
  }

  ImGui::End();
}

template <typename T>
void ModelViewerScene::ImGuiTransformComponent(T* obj, const std::string& index_str) {
  ImGui::Text("Transform");

  auto pos = obj->transform.GetPosition();
  if (ImGui::DragFloat3(("Position##" + index_str).c_str(), &pos.x, 0.01)) {
    obj->transform.SetLocalPos(pos);
  }
  static bool uniform_scale = false;
  ImGui::Checkbox("Uniform##scale", &uniform_scale);
  auto scale = obj->transform.GetScale();
  if (uniform_scale) {
    auto old_scale = scale;
    if (ImGui::DragFloat3(("Scale##" + index_str).c_str(), &scale.x, 0.001)) {
      float change = 0;
      if (old_scale.x != scale.x) {
        change = scale.x - old_scale.x;
      } else if (old_scale.y != scale.y) {
        change = scale.y - old_scale.y;
      } else {
        change = scale.z - old_scale.z;
      }
      obj->transform.SetScale(
          glm::vec3(old_scale.x + change, old_scale.y + change, old_scale.z + change));
    }
  } else {
    if (ImGui::DragFloat3(("Scale##" + index_str).c_str(), &scale.x, 0.001)) {
      obj->transform.SetScale(scale);
    }
  }
}
