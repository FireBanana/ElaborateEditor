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
	unsigned int cursorPositionX;

	std::unordered_map<int, std::string> textLines;

	ImguiRenderData() 
		: lineNumber(0), cursorPositionX(0)
	{};

	inline std::string& GetCurrentLine() { return textLines[lineNumber]; };
};

class ImguiRenderer
{
public:	
	ImguiRenderer(GLFWwindow* window, ImguiRenderData* data);
	ImguiRenderer() {};
	ImguiRenderer(const ImguiRenderer&) = delete;

	void Render();

private:
	GLFWwindow* m_Window;
	ImguiRenderData* m_Data;

	void DrawMainWindow();
	void DrawCursor();
	void DebugCursorPos(int lineNumber);
};