#pragma once
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <unordered_map>

struct ImguiRenderData
{
	uint16_t width;
	uint16_t height;

	unsigned int lineNumber;
	std::unordered_map<int, std::string> textLines;

	ImguiRenderData() { lineNumber = 0; };

	inline std::string& GetCurrentLine() { return textLines[lineNumber]; };
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