//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GROUP_SCENE_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GROUP_SCENE_HPP_

#include "src/renderer/group/Group.hpp"
#include "src/renderer/camera/Camera.hpp"

#include <memory>
class Scene {
 public:
  Transform transform;

  void AddGroup(std::unique_ptr<Group> group);
  void RemoveGroup(const Group* group);
  virtual void Update(double dt);

  inline const std::vector<std::unique_ptr<Group>>& GetGroups() const { return m_groups; }
  [[nodiscard]] inline bool GetVisible() const { return m_visible; }
  [[nodiscard]] inline bool GetWireFrame() const { return m_wireFrame; }
  inline void SetWireFrame(bool wireFrame) { m_wireFrame = wireFrame; }
  inline void SetVisible(bool visible) { m_visible = visible; }
  [[nodiscard]] inline Camera* GetCamera() const {return m_camera.get(); }
 private:
  bool m_visible{true}, m_wireFrame{false};
  std::vector<std::unique_ptr<Group>> m_groups;
 protected:
  std::unique_ptr<Camera> m_camera;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GROUP_SCENE_HPP_
