#pragma once

#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "events/Event.h"

class Window
{
private:
	struct WindowData
	{
		unsigned int Width, Height;		
	};

public:
	Window();
	void Update();
	void OnEvent(Event& event);

	inline GLFWwindow* GetWindow() { return m_Window; }
	inline ImGuiIO* GetIO() { return m_IO; }

private:
	GLFWwindow* m_Window;
	ImGuiIO* m_IO;
	WindowData m_Data;
};