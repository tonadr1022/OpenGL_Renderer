cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
  )
endif()

project("opengl_renderer")

include_directories("dep")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add source to this project's executable.
add_executable(
  opengl_renderer
  src/main.cpp
  src/utils/Input.cpp
  src/renderer/Renderer.cpp
  src/renderer/Renderer.hpp
  src/Window.cpp
  src/Window.hpp
  src/Application.cpp
  src/Application.hpp
  dep/imgui/backends/imgui_impl_opengl3.cpp
  dep/imgui/backends/imgui_impl_glfw.cpp
  dep/imgui/imgui_draw.cpp
  dep/imgui/imgui_demo.cpp
  dep/imgui/imgui.cpp
  dep/imgui/imgui_tables.cpp
  dep/imgui/imgui_widgets.cpp
  src/gl/VertexArray.cpp
  src/gl/VertexArray.hpp
  src/gl/VertexBuffer.cpp
  src/gl/VertexBuffer.hpp
  src/gl/Shader.cpp
  src/gl/Shader.hpp
  dep/KHR/khrplatform.h
  dep/glad/glad.c
  src/gl/Types.hpp
  src/gl/ElementBuffer.cpp
  src/gl/ElementBuffer.hpp
  src/utils/Logger.hpp
  src/utils/Logger.hpp
  src/renderer/Mesh.cpp
  src/renderer/Mesh.hpp
  src/camera/FPSCamera.cpp
  src/camera/OrbitCamera.cpp
  src/shapes/Cube.hpp
  src/gl/FrameBuffer.cpp
  src/gl/FrameBuffer.hpp
  src/gl/Texture.cpp
  src/gl/Texture.hpp
  src/gl/stb_image/stb_image_impl.hpp
  src/renderer/Material.cpp
  src/renderer/Material.hpp
  src/group/Object.cpp
  src/group/Object.hpp
  src/utils/HashedString.hpp
  src/group/Transform.cpp
  src/group/Transform.hpp
  src/resource/ShaderManager.cpp
  src/resource/MaterialManager.cpp
  src/utils/Utils.hpp
  src/group/Group.cpp
  src/group/Group.hpp
  src/group/Scene.cpp
  src/group/Scene.hpp
  src/resource/MeshManager.cpp
  src/resource/MeshManager.hpp
  src/resource/TextureManager.cpp
  src/resource/TextureManager.hpp
  src/scenes/PlaygroundScene.cpp
  src/scenes/PlaygroundScene.hpp
  src/camera/Camera.cpp
  src/camera/Camera.hpp
  src/Common.hpp
  src/gl/RenderBuffer.cpp
  src/gl/RenderBuffer.hpp
  src/scenes/SceneManager.cpp
  src/scenes/SceneManager.hpp
  src/imgui/ImGuiMenu.cpp
  src/imgui/ImGuiMenu.hpp
  src/camera/CameraController.cpp
  src/camera/CameraController.hpp
  src/scenes/LightingOneScene.cpp
  src/scenes/LightingOneScene.hpp
  src/renderer/Light.cpp
  src/renderer/Light.hpp
  src/scenes/ModelViewerScene.cpp
  src/scenes/ModelViewerScene.hpp
  src/renderer/Quad.cpp
  src/renderer/Quad.hpp
  src/renderer/Skybox.cpp
  src/renderer/Skybox.hpp
  src/resource/ModelManager.cpp
  src/resource/ModelManager.hpp
  src/gl/FBOContainer.cpp
  src/gl/FBOContainer.hpp
  src/utils/Utils.cpp
  src/renderer/PostProcessor.cpp
  src/renderer/PostProcessor.hpp
  src/renderer/PostProcessingEffect.hpp)

# if (CMAKE_BUILD_TYPE MATCHES Debug) add_compile_definitions(DEBUG_MODE)
# set(CMAKE_CXX_FLAGS "-g") endif ()

if(WIN32)
  add_definitions(-DWINDOWS)
else()
  add_definitions(-DUNIX)
endif()

if(APPLE)
  set(CMAKE_CXX_FLAGS -Wdeprecated-declarations)
endif()

add_subdirectory(dep/glm)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR
    ${PROJECT_SOURCE_DIR}/dep
    CACHE PATH "" FORCE)

find_package(glfw3 3.3)
if(glfw3_FOUND)
  message("GLFW found on system")
  link_directories(${GLFW_LIBRARY_DIRS})
  target_include_directories(opengl_renderer PRIVATE ${GLFW_INCLUDE_DIRS})
else()
  message("GLFW not found on system, fetching instead")
  FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master)
  FetchContent_MakeAvailable(GLFW)
  target_include_directories(opengl_renderer PRIVATE dep/glfw-src/include)
endif()

find_package(assimp)
if(assimp_FOUND)
  message("Assimp found on system")
  link_directories(${ASSIMP_LIBRARY_DIRS})
  target_include_directories(opengl_renderer PRIVATE ${ASSIMP_INCLUDE_DIRS})
else()
  message("Assimp not found on system, fetching instead")
  FetchContent_Declare(assimp
                       GIT_REPOSITORY https://github.com/assimp/assimp.git)
  set(BUILD_SHARED_LIBS
      OFF
      CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_TESTS
      OFF
      CACHE BOOL "" FORCE)
  set(ASSIMP_INJECT_DEBUG_POSTFIX
      OFF
      CACHE BOOL "" FORCE)
  set(ASSIMP_INSTALL
      OFF
      CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(assimp)
  target_include_directories(opengl_renderer PRIVATE dep/assimp-src/include)
endif()

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_link_libraries(opengl_renderer ${OPENGL_LIBRARIES} glfw glm
                      assimp::assimp)

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET opengl_renderer PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(
  TARGET opengl_renderer
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/resources
          $<TARGET_FILE_DIR:opengl_renderer>/resources)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/screenshots)

add_compile_definitions(SHADER_PATH="resources/shaders/")
add_compile_definitions(TEXTURE_PATH="resources/textures/")
