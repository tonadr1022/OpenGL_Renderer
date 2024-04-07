cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()


project("opengl_renderer")

include_directories("dep")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add source to this project's executable.
add_executable(opengl_renderer
        src/main.cpp
        src/core/Input.cpp
        src/renderer/Renderer.cpp
        src/renderer/Renderer.hpp
        src/renderer/Renderer.hpp
        src/renderer/Window.cpp
        src/renderer/Window.hpp
        src/renderer/Window.hpp
        src/renderer/Application.cpp
        src/renderer/Application.hpp
        dep/imgui/backends/imgui_impl_opengl3.cpp
        dep/imgui/backends/imgui_impl_glfw.cpp
        dep/imgui/imgui_draw.cpp
        dep/imgui/imgui_demo.cpp
        dep/imgui/imgui.cpp
        dep/imgui/imgui_tables.cpp
        dep/imgui/imgui_widgets.cpp
        src/renderer/gl/VertexArray.cpp
        src/renderer/gl/VertexArray.hpp
        src/renderer/gl/VertexBuffer.cpp
        src/renderer/gl/VertexBuffer.hpp
        src/renderer/gl/Shader.cpp
        src/renderer/gl/Shader.hpp
        dep/KHR/khrplatform.h
        dep/glad/glad.c
        src/renderer/gl/Types.hpp
        src/renderer/gl/ElementBuffer.cpp
        src/renderer/gl/ElementBuffer.hpp
        src/core/Logger.hpp
        src/core/Logger.hpp
        src/renderer/Mesh.cpp
        src/renderer/Mesh.hpp
        src/renderer/camera/FPSCamera.cpp
        src/renderer/camera/OrbitCamera.cpp
        src/renderer/shapes/Cube.hpp
        src/imgui/ImguiStyles.h
        src/imgui/ImguiStyles.h
        src/imgui/ImguiStyles.h
        src/renderer/gl/FrameBuffer.cpp
        src/renderer/gl/FrameBuffer.hpp
        src/renderer/gl/Texture.cpp
        src/renderer/gl/Texture.hpp
        src/renderer/gl/stb_image/stb_image_impl.hpp
        src/renderer/Material.cpp
        src/renderer/Material.hpp
        src/renderer/group/Object.cpp
        src/renderer/group/Object.hpp
        src/core/HashedString.hpp
        src/renderer/group/Transform.cpp
        src/renderer/group/Transform.hpp
        src/renderer/resource/ShaderManager.cpp
        src/renderer/resource/MaterialManager.cpp
        src/core/Utils.hpp
        src/renderer/group/Group.cpp
        src/renderer/group/Group.hpp
        src/renderer/group/Scene.cpp
        src/renderer/group/Scene.hpp
        src/renderer/resource/MeshManager.cpp
        src/renderer/resource/MeshManager.hpp
        src/renderer/resource/TextureManager.cpp
        src/renderer/resource/TextureManager.hpp
        src/scenes/PlaygroundScene.cpp
        src/scenes/PlaygroundScene.hpp
        src/renderer/camera/Camera.cpp
        src/renderer/camera/Camera.hpp
        src/Common.hpp
        src/renderer/gl/RenderBuffer.cpp
        src/renderer/gl/RenderBuffer.hpp
        src/scenes/SceneManager.cpp
        src/scenes/SceneManager.hpp
        src/imgui/ImGuiMenu.cpp
        src/imgui/ImGuiMenu.hpp
        src/renderer/camera/CameraController.cpp
        src/renderer/camera/CameraController.hpp
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
        src/renderer/ModelManager.cpp
        src/renderer/ModelManager.hpp
        src/renderer/gl/FBOContainer.cpp
        src/renderer/gl/FBOContainer.hpp
        src/core/Utils.cpp
        src/renderer/PostProcessor.cpp
        src/renderer/PostProcessor.hpp
        src/renderer/PostProcessingEffect.hpp
)


#if (CMAKE_BUILD_TYPE MATCHES Debug)
#    add_compile_definitions(DEBUG_MODE)
#    set(CMAKE_CXX_FLAGS "-g")
#endif ()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/screenshots)


if (WIN32)
    add_definitions(-DWINDOWS)
else ()
    add_definitions(-DUNIX)
endif ()

if (APPLE)
    set(CMAKE_CXX_FLAGS -Wdeprecated-declarations)
endif ()

# add_definitions(-Wall -v)

add_subdirectory(dep/glfw)
add_subdirectory(dep/glm)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/dep CACHE PATH "" FORCE)
FetchContent_Declare(assimp GIT_REPOSITORY https://github.com/assimp/assimp.git)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build Assimp tools" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

target_include_directories(opengl_renderer PRIVATE dep/assimp-src/include)


find_package(OpenGL REQUIRED)

target_link_libraries(opengl_renderer ${OPENGL_LIBRARIES} glfw glm assimp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET opengl_renderer PROPERTY CXX_STANDARD 20)
endif ()

add_custom_command(
        TARGET opengl_renderer PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:opengl_renderer>/resources
)

add_compile_definitions(SHADER_PATH="resources/shaders/")
add_compile_definitions(TEXTURE_PATH="resources/textures/")
