#include <GLFW/glfw3.h>
#include <imgui.h>

#include "TextWindow.h"
#include "events/CharPressedEvent.h"
#include "events/KeyPressedEvent.h"
#include "Logger.h"
#include "events/WindowResizeEvent.h"
#include "File.h"

void TextWindowRenderData::RetractAllLines(int startIndex)
{
    for (int i = startIndex; i < textLines.size(); i++)
    {
        textLines[i - 1] = textLines[i];
    }

    textLines.erase(textLines.size() - 1);
}

void TextWindowRenderData::AdvanceAllLines(int startIndex)
{
    textLines.insert({ textLines.size(), "" });

    for (int i = textLines.size() - 1; i >= startIndex; i--)
    {
        textLines[i] = textLines[i - 1];
    }
}

TextWindow::TextWindow(std::string name, const char* startShaderPath) :
	m_Name(name), m_IsFocused(false)
{
	std::string shader = File::Read(startShaderPath);

	m_IsFocused = true;

	for (int i = 0; i < shader.size(); i++)
	{
		if (shader[i] != '\n')
		{
			CharPressedEvent e;
			e.Key = shader[i];

			OnEvent(e);
		}
		else
		{
			KeyPressedEvent e;
			e.Key = GLFW_KEY_ENTER;

			OnEvent(e);
		}
	}
}

void TextWindow::Draw()
{
	if (ImGui::Begin(m_Name.c_str(), nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::SetWindowPos(ImVec2(m_RenderData.x, m_RenderData.y));
		ImGui::SetWindowSize(ImVec2(m_RenderData.width, m_RenderData.height));

		ImGui::SetWindowFontScale(1.0f);

		m_IsFocused = ImGui::IsWindowFocused();

		for (int _lineNumber = 0; _lineNumber < m_RenderData.textLines.size(); _lineNumber++)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });
			ImGui::Text(std::to_string(_lineNumber).c_str());
			ImGui::PopStyleColor();

			ImGui::SameLine();

			ImGui::Text(m_RenderData.textLines[_lineNumber].c_str());
		}

		if(m_IsFocused)
			DrawCursor();

		ImGui::End();
	}
}

std::string TextWindow::ExtractLinesToString()
{
	std::string s;

	for (int i = 0; i < m_RenderData.textLines.size(); i++)
		s += m_RenderData.textLines[i] + '\n';

	return s;
}

void TextWindow::OnEvent(Event& event)
{
	if (!m_IsFocused)
		return;

	EventHandler eHandler(event);

	eHandler.Dispatch<CharPressedEvent>([&]()
		{
			CharPressedEvent* e = static_cast<CharPressedEvent*>(&event);

			m_RenderData.GetCurrentLine().insert(
				m_RenderData.GetCurrentLine().begin() + m_RenderData.cursorPositionX,
				e->Key);

			m_RenderData.cursorPositionX++;
		});

	eHandler.Dispatch<KeyPressedEvent>([&]()
		{
			KeyPressedEvent* e = static_cast<KeyPressedEvent*>(&event);

			switch (e->Key)
			{
			case GLFW_KEY_TAB:

				OnTabPressed();
				break;

			case GLFW_KEY_BACKSPACE:

				OnBackspacePressed();
				break;

			case GLFW_KEY_ENTER:
				
				OnEnterPressed();
				break;

			case GLFW_KEY_LEFT:

				OnLeftPressed();
				break;

			case GLFW_KEY_RIGHT:

				OnRightPressed();
				break;

			case GLFW_KEY_UP:

				OnUpPressed();
				break;

			case GLFW_KEY_DOWN:

				OnDownPressed();
				break;
			}

		});
}

//============= KEY EVENTS ========================

void TextWindow::OnTabPressed()
{
	CharPressedEvent cEvent;
	cEvent.Key = GLFW_KEY_TAB;
	OnEvent(cEvent);
}

void TextWindow::OnBackspacePressed()
{
	if (m_RenderData.cursorPositionX > 0)
	{
		m_RenderData.GetCurrentLine()
			.erase(m_RenderData.GetCurrentLine().begin() + m_RenderData.cursorPositionX - 1);

		m_RenderData.cursorPositionX--;
	}
	else if (m_RenderData.lineNumber > 0)
	{
		m_RenderData.cursorPositionX = m_RenderData.GetLastLine().size();

		m_RenderData.GetLastLine() += m_RenderData.GetCurrentLine();
		m_RenderData.RetractAllLines(m_RenderData.lineNumber + 1);

		m_RenderData.lineNumber--;
	}
}

void TextWindow::OnEnterPressed()
{
	m_RenderData.lineNumber++;

	if (m_RenderData.lineNumber - 1 < m_RenderData.textLines.size() - 1)
	{
		m_RenderData.AdvanceAllLines(m_RenderData.lineNumber + 1);

		m_RenderData.GetCurrentLine() =
			m_RenderData.GetLastLine().substr(
				m_RenderData.cursorPositionX, m_RenderData.GetLastLine().size()
			);

		m_RenderData.GetLastLine().erase(
			m_RenderData.cursorPositionX,
			m_RenderData.GetLastLine().size());
	}
	else
	{
		m_RenderData.textLines.insert({ m_RenderData.lineNumber, "" });
	}

	m_RenderData.cursorPositionX = 0;
}

void TextWindow::OnLeftPressed()
{
	m_RenderData.cursorPositionX =
		(int)m_RenderData.cursorPositionX - 1 >= 0 ?
		(int)m_RenderData.cursorPositionX - 1 :
		0;
}

void TextWindow::OnRightPressed()
{
	m_RenderData.cursorPositionX =
		(int)m_RenderData.cursorPositionX + 1 <= m_RenderData.GetCurrentLine().size() ?
		(int)m_RenderData.cursorPositionX + 1 :
		m_RenderData.GetCurrentLine().size();
}

void TextWindow::OnUpPressed()
{
	m_RenderData.lineNumber =
		(int)m_RenderData.lineNumber - 1 >= 0 ?
		m_RenderData.lineNumber - 1 :
		m_RenderData.lineNumber;

	m_RenderData.cursorPositionX =
		m_RenderData.cursorPositionX >= m_RenderData.GetCurrentLine().size() ?
		m_RenderData.GetCurrentLine().size() :
		m_RenderData.cursorPositionX;
}

void TextWindow::OnDownPressed()
{
	m_RenderData.lineNumber =
		(int)m_RenderData.lineNumber + 1 < m_RenderData.textLines.size() ?
		m_RenderData.lineNumber + 1 :
		m_RenderData.lineNumber;

	m_RenderData.cursorPositionX =
		m_RenderData.cursorPositionX >= m_RenderData.GetCurrentLine().size() ?
		m_RenderData.GetCurrentLine().size() :
		m_RenderData.cursorPositionX;
}

//=================================================

void TextWindow::DrawCursor()
{
	ImVec2& padding			= ImGui::GetStyle().WindowPadding;
	ImVec2  textDrawOffset;
	ImVec2  lineSize		= ImGui::CalcTextSize(m_RenderData.GetCurrentLine().c_str());
	ImVec2	windowPos		= ImGui::GetWindowPos();

	float   charSize		= ImGui::CalcTextSize("0").x;
	float   spacing			= ImGui::GetTextLineHeightWithSpacing();

	textDrawOffset.y = m_RenderData.lineNumber * spacing + spacing + padding.y + windowPos.y;
	textDrawOffset.x = 1.0f +
		(charSize * (std::to_string(m_RenderData.lineNumber).size() + 1)) +
		(charSize * m_RenderData.cursorPositionX) +
		padding.x -
		ImGui::GetScrollX() + windowPos.x;

	ImGui::GetWindowDrawList()->AddRect(
		ImVec2(
			(charSize * m_RenderData.cursorPositionX) +
			(charSize * (std::to_string(m_RenderData.lineNumber).size() + 1)) +
			padding.x -
			ImGui::GetScrollX() + windowPos.x,

			(m_RenderData.lineNumber * spacing) + padding.y + windowPos.y),

		textDrawOffset,
		ImColor(255, 255, 255, sin(glfwGetTime() * 10) * 255)
	);
}
