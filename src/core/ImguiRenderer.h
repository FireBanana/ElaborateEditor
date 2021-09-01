#pragma once
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class ImguiRenderer
{
public:
	ImguiRenderer() {};
	ImguiRenderer(GLFWwindow* window);

	void Render(uint16_t width, uint16_t height);
	void OnDraw();

private:
	GLFWwindow* m_Window;

	void DrawMainWindow(uint16_t width, uint16_t height);
};