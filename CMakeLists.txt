cmake_minimum_required(VERSION 3.21)
project(ElaborateEditor)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)
set(CMAKE_RUNTIME_LIBRARY_DIRECTORY bin/)

set_property(GLOBAL PROPERTY USE_FOLDERS ON) 

include_directories(vendor/glad/include)
include_directories(vendor/imgui)
include_directories(vendor/imgui/backends)

include_directories(src/core/)
include_directories(src/util/)
include_directories(src/core/events/)

add_executable(ElaborateEditor 
	src/main.cpp
	src/util/Logger.h
	src/core/Window.cpp
	src/core/Window.h
	src/core/events/Event.h
	src/core/events/WindowResizeEvent.h
	src/core/events/KeyPressedEvent.h
	src/core/ImguiRenderer.cpp
	src/core/ImguiRenderer.h

	vendor/glad/src/glad.c
	vendor/imgui/imgui.cpp
	vendor/imgui/imgui_draw.cpp
	vendor/imgui/imgui_tables.cpp
	vendor/imgui/imgui_demo.cpp
	vendor/imgui/imgui_widgets.cpp
	vendor/imgui/backends/imgui_impl_glfw.cpp
	vendor/imgui/backends/imgui_impl_opengl3.cpp)

source_group("src/" FILES 
	src/main.cpp)

source_group("src/core" FILES
	src/core/Window.cpp
	src/core/Window.h
	src/core/ImguiRenderer.cpp
	src/core/ImguiRenderer.h)

source_group("src/core/events" FILES
	src/core/events/Event.h
	src/core/events/WindowResizeEvent.h
	src/core/events/KeyPressedEvent.h)

source_group("src/util" FILES 
	src/util/Logger.h)

if(WIN32)
	message("Building for Win32...")

	include_directories(vendor/glfw_win/include)
	target_link_libraries(ElaborateEditor
		 ${CMAKE_SOURCE_DIR}/vendor/glfw_win/lib/glfw3.lib
		 ${CMAKE_SOURCE_DIR}/vendor/glfw_win/lib/glfw3dll.lib)
endif(WIN32)

if(UNIX)
	message("Building for Unix...")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	add_definitions(-std=c++14)

	include_directories(vendor/glfw_mac/include)
	target_link_libraries(ElaborateEditor
		 ${CMAKE_SOURCE_DIR}/vendor/glfw_mac/lib/libglfw3.a)

	target_link_libraries(ElaborateEditor
		"-framework OpenGL" "-framework Cocoa" "-framework IOKit")
endif(UNIX)