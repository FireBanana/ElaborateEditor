#pragma once

#include<glm.hpp>

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Event.h"
#include "Camera.h"
#include "VisualObject.h"

struct ViewportRenderData
{
	uint16_t width;
	uint16_t height;
	uint16_t x;
	uint16_t y;
};

class ViewportRenderer
{
public:
	ViewportRenderer(ViewportRenderData* data);

	void Render();
	void OnEvent(Event& event);

private:

	ViewportRenderData* m_RenderData;

	VisualObject m_Viewport;
	VisualObject m_Cube;

	VertexArray m_2DVertexArray;
	VertexArray m_3DVertexArray;

	Shader m_BackgroundShader;				
	Shader m_DefaultShader;

	Camera m_Camera;
};