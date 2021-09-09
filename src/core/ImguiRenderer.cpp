#include "ImguiRenderer.h"

ImguiRenderer::ImguiRenderer(GLFWwindow* window)
    : m_Window(window)
{
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImguiRenderer::Render(ImguiRenderData& data)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //-------Rendering------

    DrawMainWindow(data);

    //----------------------

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiRenderer::DrawMainWindow(ImguiRenderData& data)
{
    if (ImGui::Begin("Main", nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(data.width, data.height));
        ImGui::TextWrapped(data.text.c_str());

        //for (int i = 0; i < data.text.size(); i++)
        //{
        ImVec2 textDrawOffset = ImGui::GetStyle().WindowPadding;
        ImVec2 paragraphSize = ImGui::CalcTextSize(data.text.c_str());

        textDrawOffset.x += paragraphSize.x;
        textDrawOffset.y += paragraphSize.y;
        
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(0, 0),
            textDrawOffset,
            ImColor::ImColor(255, 255, 255, 255)
            );
        //}

        ImGui::End();
    }
}
