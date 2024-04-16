# OpenGL Renderer

This is a basic 3D renderer written in C++ and OpenGL. Its main purpose has been for me to learn more rendering
techniques while abstracting them into
an API that allows for scene creation without making OpenGL calls. Along the way, it has improved my C++ skills,
although I'm learning that C++ (and graphics programming for that matter) is really a lifelong journey.

## Building and Running

Run tasks.py or open the project in your IDE

```
# Run in Debug mode
python tasks.py -r

# Specify Mode: [Debug, RelWithDebInfo, Release]
python tasks.py -r --mode Release

# Build Only
python tasks.py -b

# Clean
python tasks.py -c
```

Or Open the project in an IDE and build from there if these scripts don't work (Still learning CMake)

## Features

- 3D model loading
- Orbit/FPS cameras
- Skybox
- Blinn-Phong lighting: Ambient, Diffuse, Specular, Emission maps
- MSAA
- Stencil Outlining (eventually object selection)
- Dockable ImGui interface
- Screenshots

### MSAA

By rendering to a multi-sampled framebuffer and blitting (copying, and in this case, taking the average) it to a single sampled frambuffer to render to the screen or perform post-processing.

![MSAA Comparison](screenshots/msaa_comparison.png)

### Stencil Testing

Objects can be outlined with a stencil by rendering to a stencil buffer
with a shader that increases object size and draws a single color, followed by drawing the object normally. I've had issues with scaling, since some models are much larger, so it was difficult to define a single outline strength.

![Stencil Test](screenshots/stencil-test.png)

## TODO (soon to far)

- Framebuffer stack for multiple post-processing effects
- Uniform buffer objects (uniforms across shaders)
- GPU instancing
- PBR
- Commit to actually learning CMake and cross-platform compatibility (Ugh)
- Rewrite in OpenGL 4.6 with data oriented approach (batched rendering, direct state access)
- Learn Vulkan

## What I've Learned

### CMake

In my long journey moving from CLion on MacOS to Neovim on both MacOS and Linux, I realized the importance of learning CMake from the CLI and making it simple to build and run the project on different platforms.

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
