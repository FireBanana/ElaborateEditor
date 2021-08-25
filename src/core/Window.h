#pragma once

#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "events/Event.h"

class Window
{
public:
	Window(uint16_t width, uint16_t height);
	void Update();
	void OnEvent(Event& event);

	inline GLFWwindow* GetWindow() { return m_Window; }
	inline ImGuiIO* GetIO() { return m_IO; }

private:
	GLFWwindow* m_Window;
	ImGuiIO* m_IO;

	uint16_t m_Height, m_Width;
};