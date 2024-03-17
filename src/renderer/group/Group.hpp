//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GROUP_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GROUP_HPP_

#include <vector>
#include <memory>
#include "Object.hpp"

class Group {
 public:
  explicit Group(bool backFaceCull = true);
  Transform transform;
  void AddObject(std::unique_ptr<Object> object);
  void RemoveObject(const Object* object);

  [[nodiscard]] inline bool GetVisible() const { return m_visible; }
  [[nodiscard]] inline bool GetWireFrame() const { return m_wireFrame; }
  inline void SetWireFrame(bool wireFrame) { m_wireFrame = wireFrame; }
  inline void SetVisible(bool visible) { m_visible = visible; }
  void UpdateTransforms();
  [[nodiscard]] inline const std::vector<std::unique_ptr<Object>>& GetObjects() const { return m_objects; }
  bool backFaceCull = true;
 protected:
  bool m_visible{true}, m_wireFrame{false};
  std::vector<std::unique_ptr<Object>> m_objects;
};

#endif //OPENGL_RENDERER_SRC_RENDERER_GROUP_HPP_
