#include "ImguiRenderer.h"
#include "Window.h"

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

    if(ImGui::Begin("Console", nullptr, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        int _width, _height;
        glfwGetFramebufferSize(m_Window, &_width, &_height);

        ImGui::SetWindowPos(ImVec2(_width / 2.0f, _height - 100.0f));
        ImGui::SetWindowSize(ImVec2(_width / 2.0f, 100.0f));

        if (ImGui::Button("COMPILE"))
        {
            m_Data->window->GetViewportRenderer()->GetDefaultShader()->ResetSources(
                m_VertexShaderWindow.ExtractLinesToString(),
                m_FragmentShaderWindow.ExtractLinesToString());
        }

        ImGui::End();
    }

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