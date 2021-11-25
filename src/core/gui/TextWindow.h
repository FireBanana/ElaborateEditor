#pragma once

#include<unordered_map>
#include<string>

#include "Event.h"

struct TextWindowRenderData
{
	uint16_t width;
	uint16_t height;
	uint16_t x;
	uint16_t y;

	unsigned int lineNumber;
	unsigned int cursorPositionX;

	std::unordered_map<int, std::string> textLines;

	TextWindowRenderData()
		: lineNumber(0), cursorPositionX(0), width(0), height(0)
	{};

	inline std::string& GetCurrentLine() { return textLines[lineNumber]; }
	inline std::string& GetLastLine() { return textLines[lineNumber - 1 > 0 ? lineNumber - 1 : 0]; }
	inline std::string& GetNextLine()
	{
		return textLines[lineNumber + 1 < textLines.size() ? lineNumber + 1 : textLines.size()];
	}

	inline void DeleteCurrentLine() { textLines.erase(lineNumber); }

	void RetractAllLines(int startIndex);
	void AdvanceAllLines(int startIndex);
};

class TextWindow
{
public:
	TextWindow() {}
	TextWindow(std::string name, const char* startShaderPath);

	void Draw();
	void OnEvent(Event& event);

	void OnTabPressed();
	void OnBackspacePressed();
	void OnEnterPressed();
	void OnLeftPressed();
	void OnRightPressed();
	void OnUpPressed();
	void OnDownPressed();

	std::string ExtractLinesToString();

	inline void SetPosition(uint16_t x, uint16_t y) { m_RenderData.x = x; m_RenderData.y = y; };
	inline void SetSize(uint16_t w, uint16_t h) { m_RenderData.width = w; m_RenderData.height = h; };

private:

	const std::string m_Name;

	TextWindowRenderData m_RenderData;
	bool m_IsFocused;

	void DrawCursor();
};