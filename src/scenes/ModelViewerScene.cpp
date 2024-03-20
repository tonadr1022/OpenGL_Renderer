//
// Created by Tony Adriansen on 3/16/24.
//

#include "ModelViewerScene.hpp"
#include "src/renderer/resource/MaterialManager.hpp"
#include "imgui/imgui.h"
#include "src/core/Logger.hpp"
#include "src/renderer/Application.hpp"
#include "src/renderer/resource/TextureManager.hpp"
#include "src/renderer/ModelManager.hpp"

ModelViewerScene::ModelViewerScene() : Scene({10, 10, 10}, CameraController::Mode::Orbit) {
  m_skyboxNames = {"Sky 1", "Sky 2", "Church", "Winter"};
  m_activeSkyboxName = m_skyboxNames[2];
  Application::Instance().GetRenderer().SetSkyboxTexture(TextureManager::GetTexture(HashedString(m_activeSkyboxName.data())));
  auto backpack = ModelManager::CopyLoadedModel("backpack");
  auto teapot = ModelManager::CopyLoadedModel("teapot");
  auto sponza = ModelManager::CopyLoadedModel("sponza");
  auto spot = ModelManager::CopyLoadedModel("spot");

  m_modelSelectMap.emplace("Backpack", backpack.get());
  m_groups.push_back(std::move(backpack));

  m_modelSelectMap.emplace("Teapot", teapot.get());
  teapot->transform.SetScale(glm::vec3(1.0));
  m_groups.push_back(std::move(teapot));

  m_modelSelectMap.emplace("Sponza", sponza.get());
  m_groups.push_back(std::move(sponza));

  m_modelSelectMap.emplace("Spot", spot.get());
  spot->transform.SetScale(glm::vec3(10));
  spot->selected = true;
  auto spotMat = MaterialManager::GetMaterial("spotTextured");
  for (auto& obj : spot->GetObjects()) {
    obj->SetMaterial(spotMat);
  }
  m_groups.push_back(std::move(spot));


  // Lights
  glm::vec3 directionalDir = {0.4f, -0.7f, -0.7f};
  m_directionalLight = std::make_unique<DirectionalLight>(directionalDir);
  m_pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3(10, 10, 10)));

  // only one model visible
  for (auto& group : m_groups) {
    group->visible = false;
  }
  m_visibleModel = m_modelSelectMap.find("Spot")->second;
  m_visibleModel->visible = true;
  m_activeModelName = "Spot";
}

void ModelViewerScene::Update(double dt) {
  Scene::Update(dt);
}

void ModelViewerScene::OnImGui() {
  Scene::OnImGui();
  ImGui::Begin("Model Viewer");
  if (ImGui::BeginCombo("##Skybox", ("Skybox: " + m_activeSkyboxName).c_str())) {
    for (auto& name : m_skyboxNames) {
      if (ImGui::Selectable(name.data())) {
        Application::Instance().GetRenderer().SetSkyboxTexture(TextureManager::GetTexture(HashedString(name.data())));
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

  static int selectedObjectIndex = 0;

  if (ImGui::CollapsingHeader("Object Inspector")) {

    if (ImGui::BeginCombo("##ModelObjSel", ("Object: " + std::to_string(selectedObjectIndex)).c_str())) {
      for (int i = 0; i < m_visibleModel->GetObjects().size(); i++) {
        const bool isSelected = (selectedObjectIndex == i);
        if (ImGui::Selectable(std::to_string(i).c_str(), isSelected)) {
          selectedObjectIndex = i;
        }
      }
      ImGui::EndCombo();
    }

    if (selectedObjectIndex >= 0 && selectedObjectIndex < m_visibleModel->GetObjects().size()) {
      auto& obj = m_visibleModel->GetObjects()[selectedObjectIndex];
      ImGui::Checkbox("Visible", &obj->shouldDraw);
      Material* material = obj->GetMaterial();
      ImGui::Text("Material");

      static bool uniformScale = false;
      ImGui::Checkbox("Uniform##matcolors", &uniformScale);

      if (uniformScale) {
        auto oldColor = material->diffuseColor;
        if (ImGui::DragFloat3("Diffuse##selObj", &material->diffuseColor.x, 0.01, 0, 1)) {
          float change = 0;
          if (oldColor.x != material->diffuseColor.x) {
            change = material->diffuseColor.x - oldColor.x;
          } else if (oldColor.y != material->diffuseColor.y) {
            change = material->diffuseColor.y - oldColor.y;
          } else {
            change = material->diffuseColor.z - oldColor.z;
          }
          material->diffuseColor =
              {glm::clamp(oldColor.x + change, 0.0f, 1.0f), glm::clamp(oldColor.y + change, 0.0f, 1.0f),
               glm::clamp(oldColor.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Diffuse##objsel", &material->diffuseColor.x, 0.01, 0, 1);
      }

      if (uniformScale) {
        auto oldColor = material->specularColor;
        if (ImGui::DragFloat3("Specular##selObj", &material->specularColor.x, 0.01, 0, 1)) {
          float change = 0;
          if (oldColor.x != material->specularColor.x) {
            change = material->specularColor.x - oldColor.x;
          } else if (oldColor.y != material->specularColor.y) {
            change = material->specularColor.y - oldColor.y;
          } else {
            change = material->specularColor.z - oldColor.z;
          }
          material->specularColor =
              {glm::clamp(oldColor.x + change, 0.0f, 1.0f), glm::clamp(oldColor.y + change, 0.0f, 1.0f),
               glm::clamp(oldColor.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Specular##objsel", &material->specularColor.x, 0.01, 0, 1);
      }

      if (uniformScale) {
        auto oldColor = material->ambientColor;
        if (ImGui::DragFloat3("Ambient##selObj", &material->ambientColor.x, 0.01, 0, 1)) {
          float change = 0;
          if (oldColor.x != material->ambientColor.x) {
            change = material->ambientColor.x - oldColor.x;
          } else if (oldColor.y != material->ambientColor.y) {
            change = material->ambientColor.y - oldColor.y;
          } else {
            change = material->ambientColor.z - oldColor.z;
          }
          material->ambientColor ={glm::clamp(oldColor.x + change, 0.0f, 1.0f), glm::clamp(oldColor.y + change, 0.0f, 1.0f),
               glm::clamp(oldColor.z + change, 0.0f, 1.0f)};
        }
      } else {
        ImGui::DragFloat3("Ambient##objsel", &material->ambientColor.x, 0.01, 0, 1);
      }
      ImGuiTransformComponent(obj.get(), "-1");
    }
  }

  ImGui::End();

}

template<typename T>
void ModelViewerScene::ImGuiTransformComponent(T* obj, const std::string& iStr) {
  ImGui::Text("Transform");

  auto pos = obj->transform.GetPosition();
  if (ImGui::DragFloat3(("Position##" + iStr).c_str(), &pos.x, 0.01)) {
    obj->transform.SetLocalPos(pos);

  }
  static bool uniformScale = false;
  ImGui::Checkbox("Uniform##scale", &uniformScale);
  auto scale = obj->transform.GetScale();
  if (uniformScale) {
    auto oldScale = scale;
    if (ImGui::DragFloat3(("Scale##" + iStr).c_str(), &scale.x, 0.001)) {
      float change = 0;
      if (oldScale.x != scale.x) {
        change = scale.x - oldScale.x;
      } else if (oldScale.y != scale.y) {
        change = scale.y - oldScale.y;
      } else {
        change = scale.z - oldScale.z;
      }
      obj->transform.SetScale(glm::vec3(oldScale.x + change, oldScale.y + change, oldScale.z + change));
    }
  } else {
    if (ImGui::DragFloat3(("Scale##" + iStr).c_str(), &scale.x, 0.001)) {
      obj->transform.SetScale(scale);

    }
  }
}

