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
  Scene();
  explicit Scene(HashedString name);
  ~Scene() = default;
  void AddGroup(std::unique_ptr<Group> group);
  void RemoveGroup(const Group* group);
  virtual void Update(double dt);

  inline const std::vector<std::unique_ptr<Group>>& GetGroups() const { return m_groups; }
  inline HashedString GetName() const { return m_name; }
 protected:
  HashedString m_name;
  std::vector<std::unique_ptr<Group>> m_groups;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GROUP_SCENE_HPP_
