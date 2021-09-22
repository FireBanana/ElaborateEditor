#include "ImguiRenderer.h"
#include <Logger.h>
#include <GLFW/glfw3.h>
#include <math.h>

ImguiRenderer::ImguiRenderer(GLFWwindow* window, ImguiRenderData* m_Data)
    : m_Window(window), m_Data(m_Data)
{
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImguiRenderer::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //-------Rendering------

    DrawMainWindow();

    //----------------------

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiRenderer::DrawMainWindow()
{
    if (ImGui::Begin("Main", false,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(m_Data->width, m_Data->height));

        for (int lineNumber = 0; lineNumber < m_Data->textLines.size(); lineNumber++)
        {
            ImGui::Text(m_Data->textLines[lineNumber].c_str());
        }        

        DrawCursor();

        ImGui::End();
    }
}

void ImguiRenderer::DrawCursor()
{
    ImVec2& padding =       ImGui::GetStyle().WindowPadding;
    ImVec2  textDrawOffset;
    ImVec2  lineSize =      ImGui::CalcTextSize(m_Data->GetCurrentLine().c_str());
    float   charSize =      ImGui::CalcTextSize("0").x;
    float   spacing =       ImGui::GetTextLineHeightWithSpacing();

    textDrawOffset.y =      m_Data->lineNumber * spacing + spacing + padding.y;
    textDrawOffset.x =      1.0f + charSize * m_Data->cursorPositionX + padding.x - ImGui::GetScrollX();
 
    ImGui::GetWindowDrawList()->AddRect(
        ImVec2(
            charSize * m_Data->cursorPositionX + padding.x - ImGui::GetScrollX(),
            (m_Data->lineNumber * spacing) + padding.y),
        textDrawOffset,
        ImColor(255, 255, 255, sin(glfwGetTime() * 10) * 255)
    );
}

void ImguiRenderer::DebugCursorPos(int lineNumber)
{
    ImVec2& padding =       ImGui::GetStyle().WindowPadding;
    ImVec2  textDrawOffset;
    ImVec2  lineSize =      ImGui::CalcTextSize(m_Data->GetCurrentLine().c_str());
    float   charSize =      lineSize.x / m_Data->GetCurrentLine().size();
    float   spacing =       ImGui::GetTextLineHeightWithSpacing();

    textDrawOffset.y =      lineNumber * spacing + spacing + padding.y;

    for (int i = 0; i < m_Data->GetCurrentLine().size(); i++)
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
