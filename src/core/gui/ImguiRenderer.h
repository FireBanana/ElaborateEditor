#pragma once
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <unordered_map>

#include "TextWindow.h"

class Window;

struct ImguiWindowData
{
	Window* window;
	uint16_t width, height;
};

class ImguiRenderer
{
public:
	ImguiRenderer(GLFWwindow* window, ImguiWindowData* data);
	ImguiRenderer() {};
	ImguiRenderer(const ImguiRenderer&) = delete;

	void Render();
	void OnEvent(Event& event);

private:
	GLFWwindow* m_Window;
	ImguiWindowData* m_Data;

	TextWindow m_VertexShaderWindow;
	TextWindow m_FragmentShaderWindow;
};