#include "ImguiRenderer.h"
#include <Logger.h>

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

        ImVec2& padding = ImGui::GetStyle().WindowPadding;

        for (auto& line : data.textLines)
        {
            ImVec2 textDrawOffset;
            ImVec2 lineSize = ImGui::CalcTextSize(line.second.c_str());
            ImGui::Text(line.second.c_str());

            textDrawOffset.x = lineSize.x + padding.x;
            textDrawOffset.y = line.first * lineSize.y + lineSize.y + padding.y;

            ImGui::GetWindowDrawList()->AddRect(
                ImVec2(0, line.first * lineSize.y + padding.y),
                textDrawOffset,
                ImColor::ImColor(255, 255, 255, 255)
            );
        }        

        ImGui::End();
    }
}
