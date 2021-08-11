cmake_minimum_required(VERSION 3.21)
project(ElaborateEditor)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)
set(CMAKE_RUNTIME_LIBRARY_DIRECTORY bin/)

add_executable(ElaborateEditor src/main.cpp vendor/glad/src/glad.c)
include_directories(vendor/glad/include)

if(WIN32)
	include_directories(vendor/glfw_win/include)
	target_link_libraries(ElaborateEditor
		 ${CMAKE_SOURCE_DIR}/vendor/glfw_win/lib/glfw3.lib
		 ${CMAKE_SOURCE_DIR}/vendor/glfw_win/lib/glfw3dll.lib)
endif(WIN32)

if(UNIX)
	include_directories(vendor/glfw_mac/include)
	target_link_libraries(ElaborateEditor
		 ${CMAKE_SOURCE_DIR}/vendor/glfw_mac/lib/libglfw3.a)
endif(UNIX)