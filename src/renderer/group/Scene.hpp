//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GROUP_SCENE_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GROUP_SCENE_HPP_

#include "src/renderer/group/Group.hpp"
#include "src/renderer/camera/Camera.hpp"
#include "src/renderer/Light.hpp"
#include <memory>

class Scene {
 public:
  Scene();
  explicit Scene(HashedString name);
  Scene(HashedString name, const glm::vec3& defaultCameraPos);
  virtual ~Scene() = default;
  virtual void Update(double dt);
  void ImGuiLights();
  virtual void OnImGui();

  [[nodiscard]] inline const std::vector<std::unique_ptr<Group>>& GetGroups() const { return m_groups; }
  [[nodiscard]] const DirectionalLight* GetDirectionalLight() const;
  [[nodiscard]] inline const std::vector<std::unique_ptr<PointLight>>* GetPointLights() const { return &m_pointLights; }
  [[nodiscard]] inline const std::vector<std::unique_ptr<SpotLight>>* GetSpotLights() const { return &m_spotLights; }

  [[nodiscard]] inline HashedString GetName() const { return m_name; }

  glm::vec3 defaultCameraPosition;
 protected:
  void AddGroup(std::unique_ptr<Group> group);
  void RemoveGroup(const Group* group);

  HashedString m_name;
  std::vector<std::unique_ptr<SpotLight>> m_spotLights;
  std::vector<std::unique_ptr<PointLight>> m_pointLights;
  std::unique_ptr<DirectionalLight> m_directionalLight = nullptr;
  std::vector<std::unique_ptr<Group>> m_groups;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GROUP_SCENE_HPP_
