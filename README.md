# OpenGL Renderer

This is a basic 3D renderer written in C++ and OpenGL. Its main purpose has been for me to learn more rendering techniques while abstracting them into
an API that allows for scene creation without making OpenGL calls. Along the way, it has improved my C++ skills,
although I'm learning that C++ (and graphics programming for that matter) is really a lifelong journey.

## Features

- 3D model loading
- Orbit/FPS cameras
- Skybox
- Blinn-Phong lighting: Ambient, Diffuse, Specular, Emission maps
- GUI to interact with scenes

## TODO (from soon to far)
- Framebuffer stack for multiple post-processing effects
- Uniform buffer objects (uniforms across shaders)
- GPU instancing
- Antialiasing (MSAA)
- PBR
- Commit to actually learning CMake and cross-platform compatibility (Ugh)
- Rewrite in OpenGL 4.6 with data oriented approach (batched rendering, direct state access)
- Learn Vulkan

## Dependencies
- [GLFW](https://github.com/glfw/glfw): window creation and OpenGL context management
- [GLAD](https://glad.dav1d.de/): OpenGL function loader
- [stb_image](https://github.com/nothings/stb): Image loading
- [GLM](https://github.com/g-truc/glm): Math
- [ImGui](https://github.com/ocornut/imgui): GUI

## References
- [LearnOpenGL](https://learnopengl.com/Introduction): Much learning and some textures
- [GEngine](https://github.com/JuanDiegoMontoya/Gengine/tree/37739ecfcb608f6b282b36ed8a962fac968b1487):
  Inspiration for input handling with GLFW, and the HashedString
  class which itself was inspired by [Entt](https://github.com/skypjack/entt).
- [ThinMatrix](https://www.youtube.com/watch?v=VS8wlS9hF8E&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP):
  A few OpenGL techniquesÎ


