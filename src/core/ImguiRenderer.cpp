#include "ImguiRenderer.h"

ImguiRenderer::ImguiRenderer(GLFWwindow* window)
    : m_Window(window)
{
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImguiRenderer::Render(uint16_t width, uint16_t height)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //-------Rendering------

    DrawMainWindow(width, height);

    //----------------------

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiRenderer::DrawMainWindow(uint16_t width, uint16_t height)
{
    if (ImGui::Begin("Main", nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(width, height));
        ImGui::TextWrapped("This is some wrappeed text matey, lets see if it wraps");
        ImGui::End();
    }
}
