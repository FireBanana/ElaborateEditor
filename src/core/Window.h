#pragma once

#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "events/Event.h"
#include "gui/ImguiRenderer.h"
#include "ViewportRenderer.h"

class Window
{
public:
	Window(uint16_t width, uint16_t height);
	Window(const Window&) = delete;
	~Window();

	void Update();
	void OnEvent(Event& event);

	inline GLFWwindow* GetWindow() { return m_Window; }
	inline ImGuiIO* GetIO() { return m_IO; }

	inline ImguiRenderer* GetImguiRenderer() { return m_ImguiRenderer; }
	inline ViewportRenderer* GetViewportRenderer() { return m_ViewportRenderer; }

private:
	GLFWwindow* m_Window;
	ImGuiIO* m_IO;
	ImguiRenderer* m_ImguiRenderer;
	ViewportRenderer* m_ViewportRenderer;

	ImguiWindowData m_ImguiWindowData;
	ViewportRenderData m_ViewportRenderData;
};