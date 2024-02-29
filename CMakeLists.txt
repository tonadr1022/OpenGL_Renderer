cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("opengl_renderer")

include_directories("dep")

# Add source to this project's executable.
add_executable (opengl_renderer src/main.cpp src/renderer/Input.cpp src/glad.c
		src/renderer/Common.hpp
		src/renderer/gl/Renderer.cpp
		src/renderer/gl/Renderer.hpp
		src/renderer/gl/Renderer.hpp
		src/renderer/Window.cpp
		src/renderer/Window.hpp
		src/renderer/Window.hpp
		src/renderer/Application.cpp
		src/renderer/Application.hpp
		dep/imgui/imgui_impl_opengl3.cpp
		dep/imgui/imgui_impl_glfw.cpp
		dep/imgui/imgui_draw.cpp
		dep/imgui/imgui_demo.cpp
		dep/imgui/imgui.cpp
		dep/imgui/imgui_tables.cpp
		dep/imgui/imgui_widgets.cpp)

add_subdirectory(dep/glfw)
add_subdirectory(dep/glm)

find_package(OpenGL REQUIRED)

target_link_libraries(opengl_renderer ${OPENGL_LIBRARIES} glfw glm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET opengl_renderer PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(
	TARGET opengl_renderer PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_SOURCE_DIR}/resources
	$<TARGET_FILE_DIR:opengl_renderer>/resources
)