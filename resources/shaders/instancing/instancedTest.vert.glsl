#version 410 core
//
// layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aNormal;
// layout(location = 2) in vec2 aTexCoord;
// layout(location = 3) in mat4 aModelMatrix;
//
uniform mat4 u_VP;
// // uniform mat4 u_Model;
//
// void main() {
//     vec4 worldPos = aModelMatrix * vec4(aPos, 1.0);
//     gl_Position = u_VP * worldPos;
// }

layout (location = 0) in vec3 Vertex;
layout (location = 3) in mat4 Matrix;
void main()
{
  gl_Position = u_VP * Matrix*vec4(Vertex.x, Vertex.y, Vertex.z, 1.0);
};
