#include <cstdint>
#include <vector>

#include "glm/fwd.hpp"
#include "src/group/Group.hpp"

class InstancedGroup {
  friend class Mesh;
  friend class Renderer;

 public:
  InstancedGroup(const Group* group, const std::vector<glm::mat4>& model_matrices);
  ~InstancedGroup();

  uint32_t m_matrix_buffer_id;
  uint32_t m_num_instances;
  const Group* m_model;
};
