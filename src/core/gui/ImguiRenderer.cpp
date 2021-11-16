#include "ImguiRenderer.h"
#include <Logger.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <string>
#include <events/WindowResizeEvent.h>

ImguiRenderer::ImguiRenderer(GLFWwindow* window, ImguiWindowData* m_Data)
    : m_Window(window), m_Data(m_Data),
    m_VertexShaderWindow("Vertex", "shaders\\default_vertex.vs"),
    m_FragmentShaderWindow("Fragment", "shaders\\default_fragment.fs")
{
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    int _width, _height;
    glfwGetFramebufferSize(window, &_width, &_height);

    m_VertexShaderWindow.SetPosition(0, 0);
    m_FragmentShaderWindow.SetPosition(0, _height / 2.0f);

    m_VertexShaderWindow.SetSize(_width / 2.0f, _height / 2.0f);
    m_FragmentShaderWindow.SetSize(_width / 2.0f, _height / 2.0f);
}

void ImguiRenderer::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //-------Rendering------

    m_VertexShaderWindow.Draw();
    m_FragmentShaderWindow.Draw();

    //----------------------

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiRenderer::OnEvent(Event& event)
{
    EventHandler eHandler(event);

    eHandler.Dispatch<WindowResizeEvent>([&]()
	{
		WindowResizeEvent* e = static_cast<WindowResizeEvent*>(&event);

        m_FragmentShaderWindow.SetPosition(0, e->Height / 2.0f);

        m_VertexShaderWindow.SetSize(e->Width / 2.0f, e->Height / 2.0f);
        m_FragmentShaderWindow.SetSize(e->Width / 2.0f, e->Height / 2.0f);
	});

    m_FragmentShaderWindow.OnEvent(event);
    m_VertexShaderWindow.OnEvent(event);
}