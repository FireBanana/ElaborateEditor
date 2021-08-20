#pragma once

#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Window
{
public:
	Window();
	void Update();

	inline GLFWwindow* GetWindow() { return m_Window; }

private:
	GLFWwindow* m_Window;
	ImGuiIO* m_IO;
};