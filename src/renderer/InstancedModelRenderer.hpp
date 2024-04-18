#include <cstdint>
#include <vector>

#include "glm/fwd.hpp"
#include "src/group/Group.hpp"

class InstancedModelRenderer {
  friend class Mesh;
  friend class Renderer;

 public:
  InstancedModelRenderer(const Group* model, const std::vector<glm::mat4>& model_matrices);
  ~InstancedModelRenderer();

  uint32_t m_matrix_buffer_id;
  uint32_t m_num_instances;
  const Group* m_model;
};
