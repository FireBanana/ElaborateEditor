#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glm.hpp>

#include "TextWindow.h"
#include "events/CharPressedEvent.h"
#include "events/KeyPressedEvent.h"
#include "events/MouseClickedEvent.h"
#include "events/MousePositionEvent.h"
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
		
		m_RenderData.scrollY = ImGui::GetScrollY();
		m_RenderData.scrollX = ImGui::GetScrollX();

		m_IsFocused = ImGui::IsWindowFocused();

		for (int _lineNumber = 0; _lineNumber < m_RenderData.textLines.size(); _lineNumber++)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });
			ImGui::Text(std::to_string(_lineNumber).c_str());
			ImGui::PopStyleColor();

			ImGui::SameLine();

			ImGui::Text(m_RenderData.textLines[_lineNumber].c_str());
		}

		if (m_IsFocused)
		{
			DrawCursor();
			DrawHighlight();
		}

		ImGui::End();
	}
}

void TextWindow::DrawCursor()
{
	ImVec2& padding = ImGui::GetStyle().WindowPadding;
	ImVec2  textDrawOffset;
	ImVec2  lineSize = ImGui::CalcTextSize(m_RenderData.GetCurrentLine().c_str());
	ImVec2	windowPos = ImGui::GetWindowPos();

	float   charSize = ImGui::CalcTextSize("0").x;
	float   spacing = ImGui::GetTextLineHeightWithSpacing();

	float xStart = (charSize * m_RenderData.cursorPositionX) +
					(charSize * (std::to_string(m_RenderData.lineNumber).size() + 1)) +
					padding.x -
					ImGui::GetScrollX() + windowPos.x;

	float yStart = (m_RenderData.lineNumber * spacing) + padding.y + windowPos.y - ImGui::GetScrollY();

	textDrawOffset.y = yStart + spacing;
	textDrawOffset.x = 1.0f + xStart;

	ImGui::GetWindowDrawList()->AddRect(
		ImVec2(xStart, yStart),
		textDrawOffset,
		ImColor(255, 255, 255, sin(glfwGetTime() * 10) * 255)
	);
}

void TextWindow::DrawHighlight()
{
	//TODO add all padding/margins
	if (m_RenderData.cursorPositionX == m_RenderData.highlightCursorPosition &&
		m_RenderData.lineNumber == m_RenderData.highlightLineNumber)
		return;

	float charSize = ImGui::CalcTextSize("0").x;

	ImGui::GetWindowDrawList()->AddRectFilled(
		ImVec2(m_RenderData.highlightCursorPosition * charSize, m_RenderData.highlightLineNumber * charSize),
		ImVec2(m_RenderData.cursorPositionX * charSize, m_RenderData.lineNumber * charSize),
		ImColor::ImColor(1.0f, 1.0f, 1.0f, 0.2f));
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

			//Clear any highlighting previously
			m_RenderData.ResetHighlights();
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

			//Clear any highlighting previously
			m_RenderData.ResetHighlights();

		});

	eHandler.Dispatch<MousePositionEvent>([&]()
		{
			MousePositionEvent* e = static_cast<MousePositionEvent*>(&event);

			m_RenderData.mouseX = e->XPosition;
			m_RenderData.mouseY = e->YPosition;
		});

	eHandler.Dispatch<MouseClickedEvent>([&]() 
		{
			MouseClickedEvent* e = static_cast<MouseClickedEvent*>(&event);

			if (e->Button == GLFW_MOUSE_BUTTON_LEFT)
			{
				if (e->Action == GLFW_PRESS)
				{
					R_POINT coords = MousePositionToCursor(m_RenderData.mouseX, m_RenderData.mouseY);

					m_RenderData.lineNumber = coords.first;
					m_RenderData.cursorPositionX = coords.second;

					m_RenderData.highlightLineNumber = coords.first;
					m_RenderData.highlightCursorPosition = coords.second;
				}
				else if (e->Action == GLFW_RELEASE)
				{
					R_POINT coords = MousePositionToCursor(m_RenderData.mouseX, m_RenderData.mouseY);

					m_RenderData.lineNumber = coords.first;
					m_RenderData.cursorPositionX = coords.second;
				}
			}
		});
}

//============= KEY EVENTS ========================
//TODO add highlight visuals
void TextWindow::OnTabPressed()
{
	for (int i = 0; i < 4; i++)
	{
		m_RenderData.GetCurrentLine().insert(
			m_RenderData.GetCurrentLine().begin() + m_RenderData.cursorPositionX,
			' ');

		m_RenderData.cursorPositionX++;
	}
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
	if (m_RenderData.cursorPositionX != 0)
	{
		m_RenderData.cursorPositionX--;
	}
	else if(m_RenderData.lineNumber > 0)
	{
		m_RenderData.lineNumber--;
		m_RenderData.cursorPositionX = m_RenderData.GetCurrentLine().size();
	}
}

void TextWindow::OnRightPressed()
{
	if (m_RenderData.cursorPositionX != m_RenderData.GetCurrentLine().size())
	{
		m_RenderData.cursorPositionX++;
	}
	else if (m_RenderData.lineNumber < m_RenderData.textLines.size() - 1)
	{
		m_RenderData.lineNumber++;
		m_RenderData.cursorPositionX = 0;
	}
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

R_POINT TextWindow::MousePositionToCursor(uint16_t x, uint16_t y)
{
	float	yPos	  = m_RenderData.y;
	float   xPos	  = m_RenderData.x;
	float   spacing   = ImGui::GetTextLineHeightWithSpacing();
	
	int line = (y - yPos + m_RenderData.scrollY - ImGui::GetStyle().WindowPadding.y) / (spacing);
	int pos = 
		( 
			x - 1.0f - ImGui::GetStyle().WindowPadding.x - 
			ImGui::CalcTextSize(std::to_string(line).c_str()).x + m_RenderData.scrollX
		) 
		/ ImGui::CalcTextSize("0").x;

	line = glm::clamp(line, 0, (int)m_RenderData.textLines.size() - 1);
	pos = glm::clamp(pos, 0, (int)m_RenderData.textLines[line].size());

	return R_POINT{line, pos};
}
