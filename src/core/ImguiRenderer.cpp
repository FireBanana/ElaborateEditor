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

        for (int lineNumber = 0; lineNumber < data.textLines.size(); lineNumber++)
        {
            ImVec2 textDrawOffset;
            ImVec2 lineSize = ImGui::CalcTextSize(data.textLines[lineNumber].c_str());
            float charSize = lineSize.x / data.textLines[lineNumber].size();
            ImGui::Text(data.textLines[lineNumber].c_str());
            float spacing = ImGui::GetTextLineHeightWithSpacing();
            
            textDrawOffset.y = lineNumber * spacing + spacing + padding.y;

            for(int i = 0; i < data.textLines[lineNumber].size(); i++)
            {
                textDrawOffset.x = charSize + charSize * i + padding.x;
                
                ImGui::GetWindowDrawList()->AddRect(
                    ImVec2(
                           charSize * i + padding.x,
                           (lineNumber * spacing) + padding.y),
                    textDrawOffset,
                    ImColor(255, 255, 255, 255)
                );
            }
        }        

        ImGui::End();
    }
}
