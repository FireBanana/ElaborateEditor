#pragma once
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct ImguiRenderData
{
	uint16_t width;
	uint16_t height;

	std::string text;
};

class ImguiRenderer
{
public:
	ImguiRenderer() {};
	ImguiRenderer(GLFWwindow* window);

	void Render(ImguiRenderData& data);

private:
	GLFWwindow* m_Window;

	void DrawMainWindow(ImguiRenderData& datat);
};